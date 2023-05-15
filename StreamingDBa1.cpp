#include "StreamingDBa1.h"

streaming_database::~streaming_database()
= default;

StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
    if (movieId <= 0 || views < 0 || genre == Genre::NONE) {
        return StatusType::INVALID_INPUT;
    }

    //TODO: consider smart pointers
    Movie* movie;
    try {
        movie = new Movie(movieId, genre, views, vipOnly);
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    try {
        if (this->movies.insert(movieId, movie)) {
            delete movie;
            return StatusType::FAILURE;
        }
        this->moviesByGenre[static_cast<int>(genre)].insert(*movie, movie);
        this->moviesByGenre[static_cast<int>(Genre::NONE)].insert(*movie, movie);
    } catch (std::bad_alloc& e) {
        delete movie;
        return StatusType::ALLOCATION_ERROR;
    }

	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
    if (movieId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Node<int, Movie*>* movieNode = this->movies.find(movieId, this->movies.getRoot());
    if (!movieNode)
        return StatusType::FAILURE;

    try {
        Movie* movie = movieNode->getValue();
        moviesByGenre[static_cast<int>(movie->getGenre())].remove(*movie);
        moviesByGenre[static_cast<int>(Genre::NONE)].remove(*movie);
        this->movies.remove(movieId);
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user(int userId, bool isVip)
{
    if (userId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    User* user;
    try {
        user = new User(userId, isVip);
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    try {
        if (this->users.insert(userId, user)) {
            delete user;
            return StatusType::FAILURE;
        }
    } catch (std::bad_alloc& e) {
        delete user;
        return StatusType::ALLOCATION_ERROR;
    }

	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
    if (userId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Node<int, User*>* userNode = this->users.find(userId, this->users.getRoot());
    if (!userNode)
        return StatusType::FAILURE;

    User* user = userNode->getValue();
    if (user->isInGroup()) {
        Group* group = this->groups.find(user->getGroupId(), this->groups.getRoot())->getValue();
        group->removeUserViews(user->getViewsByGenre());
        group->removeUser(userId);
    }

    try {
        this->users.remove(userId);
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::add_group(int groupId)
{
    if (groupId <= 0)
        return StatusType::INVALID_INPUT;

    Group* group;
    try {
        group = new Group(groupId);
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    try {
        if (this->groups.insert(groupId, group)) {
            delete group;
            return StatusType::FAILURE;
        }
    } catch (std::bad_alloc& e) {
        delete group;
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_group(int groupId)
{
    if (groupId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Group* group = this->groups.find(groupId, this->groups.getRoot())->getValue();
    if (group == nullptr)
        return StatusType::FAILURE;
    else
        group->updateUsersBeforeDelete();

    try {
        if (!this->groups.remove(groupId)) {
            return StatusType::FAILURE;
        }
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
    if (userId <= 0 || groupId <= 0)
        return StatusType::INVALID_INPUT;

    //TODO: check for allocation failure???
    Node<int, User*>* userNode = this->users.find(userId, users.getRoot());
    if (userNode == nullptr || userNode->getValue()->isInGroup())
        return StatusType::FAILURE;

    Node<int, Group*>* groupNode = this->groups.find(groupId, groups.getRoot());
    if (groupNode == nullptr)
        return StatusType::FAILURE;

    User* user = userNode->getValue();
    Group* group = groupNode->getValue();
    group->updateViews(user->getViewsByGenre());
    group->insertUser(user);
    user->setInGroup(group->getId(), group->getViewsByGenre(), group->getTotalViews());


    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
    if (userId <= 0 || movieId <= 0)
        return StatusType::INVALID_INPUT;

    Node<int, User*>* userNode = this->users.find(userId, users.getRoot());
    if (userNode == nullptr)
        return StatusType::FAILURE;

    Node<int, Movie*>* movieNode = this->movies.find(movieId, movies.getRoot());
    if (movieNode == nullptr || (movieNode->getValue()->isVipOnly() && !userNode->getValue()->isVipUser()))
        return StatusType::FAILURE;

    User* user = userNode->getValue();
    Movie* movie = movieNode->getValue();
    moviesByGenre[static_cast<int>(movie->getGenre())].remove(*movie);
    moviesByGenre[static_cast<int>(Genre::NONE)].remove(*movie);
    user->watchMovie(movieNode->getValue()->getGenre());
    movieNode->getValue()->updateViews(1);
    if (user->isInGroup()) {
        Group* group = this->groups.find(user->getGroupId(), groups.getRoot())->getValue();
        group->updateViewsSolo(movieNode->getValue()->getGenre());
    }



    moviesByGenre[static_cast<int>(movie->getGenre())].insert(*movie, movie);
    moviesByGenre[static_cast<int>(Genre::NONE)].insert(*movie, movie);
    //---------------------------------------------

    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
    if (groupId <= 0 || movieId <= 0)
        return StatusType::INVALID_INPUT;

    //TODO: maybe it is possible to make separate function for this
    Node<int, Group*>* groupNode = this->groups.find(groupId, groups.getRoot());
    if (groupNode == nullptr || groupNode->getValue()->getSize() == 0)
        return StatusType::FAILURE;

    Node<int, Movie*>* movieNode = this->movies.find(movieId, movies.getRoot());
    if (movieNode == nullptr || (movieNode->getValue()->isVipOnly() && !groupNode->getValue()->isVipGroup()))
        return StatusType::FAILURE;
    //---------------------------------------------

    //watch movie
    Movie* movie = movieNode->getValue();
    Group* group = groupNode->getValue();

    moviesByGenre[static_cast<int>(movie->getGenre())].remove(*movie);
    moviesByGenre[static_cast<int>(Genre::NONE)].remove(*movie);

    group->updateGroupViews(movie->getGenre());
    movie->updateViews(group->getSize());

    moviesByGenre[static_cast<int>(movie->getGenre())].insert(*movie, movie);
    moviesByGenre[static_cast<int>(Genre::NONE)].insert(*movie, movie);
    //---------------------------------------------
	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
    if (genre == Genre::NONE)
        return this->movies.getSize();
    else
        return this->moviesByGenre[static_cast<int>(genre)].getSize();

}

StatusType streaming_database::get_all_movies(Genre genre, int* const output)
{
    if (output == nullptr)
        return StatusType::INVALID_INPUT;

    if (genre == Genre::NONE) {
        if (this->movies.getSize() == 0)
            return StatusType::FAILURE;
    } else {
        if (this->moviesByGenre[static_cast<int>(genre)].getSize() == 0)
            return StatusType::FAILURE;
    }

    int pos = 0;
    Node<Movie, Movie*>* root = moviesByGenre[static_cast<int>(genre)].getRoot();
    moviesByGenre[static_cast<int>(genre)].inOrder(root, output, pos);

    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
    if (userId <= 0)
        return StatusType::INVALID_INPUT;

    Node<int, User*>* userNode = this->users.find(userId, users.getRoot());
    if (userNode == nullptr)
        return StatusType::FAILURE;

    int views = 0;
    User* user = userNode->getValue();
    if (genre == Genre::NONE) {
        views += user->getTotalViews();
        if (user->isInGroup()) {
            Group* group = this->groups.find(user->getGroupId(), groups.getRoot())->getValue();
            views += group->getTotalViews() - user->getViewsBeforeJoined();
        }
    } else {
        views += (user->getViewsByGenre(genre));
        if (user->isInGroup()) {
            Group* group = this->groups.find(user->getGroupId(), groups.getRoot())->getValue();
            views += group->getViewsByGenre(genre) - user->getViewsBeforeJoinedByGenre(genre);
        }
    }
    output_t<int> output(views);
    return output;
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
    if (userId <= 0 || movieId <= 0 || rating < 0 || rating > 100) {
        return StatusType::INVALID_INPUT;
    }

    //TODO: consider making separate function for this
    Node<int, User*>* userNode = this->users.find(userId, users.getRoot());
    if (userNode == nullptr)
        return StatusType::FAILURE;

    Node<int, Movie*>* movieNode = this->movies.find(movieId, movies.getRoot());
    if (movieNode == nullptr || (movieNode->getValue()->isVipOnly() && !userNode->getValue()->isVipUser()))
        return StatusType::FAILURE;
    //---------------------------------------------

    Movie* movie = movieNode->getValue();
    if (rating > 0) {
        //TODO: consider making separate function for this i.e. updateGenreTree
        moviesByGenre[static_cast<int>(movie->getGenre())].remove(*movie);
        moviesByGenre[static_cast<int>(Genre::NONE)].remove(*movie);
        movie->updateRating(rating);
        moviesByGenre[static_cast<int>(movie->getGenre())].insert(*movie, movie);
        moviesByGenre[static_cast<int>(Genre::NONE)].insert(*movie, movie);
    }
    //---------------------------------------------
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
	if (groupId <= 0)
        return StatusType::INVALID_INPUT;

    Node<int, Group*>* groupNode = this->groups.find(groupId, groups.getRoot());

    if (groupNode == nullptr || groupNode->getValue()->getSize() == 0)
        return StatusType::FAILURE;

    Genre favoriteGenre = groupNode->getValue()->getFavoriteGenre();
    if (favoriteGenre == Genre::NONE || this->moviesByGenre[static_cast<int>(favoriteGenre)].getSize() == 0)
        return StatusType::FAILURE;

    //reversed order in genre trees//
    Movie* movie = this->moviesByGenre[static_cast<int>(favoriteGenre)].getMinNodeValue();
    return movie->getId();
}

