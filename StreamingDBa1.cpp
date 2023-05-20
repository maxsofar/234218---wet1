#include "StreamingDBa1.h"

using userSharedPtr = std::shared_ptr<User>;
using groupSharedPtr = std::shared_ptr<Group>;
using movieSharedPtr = std::shared_ptr<Movie>;
using MovieNode = Node<int, movieSharedPtr>;
using UserNode = Node<int, userSharedPtr>;
using GroupNode = Node<int, groupSharedPtr>;

streaming_database::~streaming_database()
= default;

StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
    if (movieId <= 0 || views < 0 || genre == Genre::NONE)
        return StatusType::INVALID_INPUT;

    movieSharedPtr movie;
    try {
        movie = std::make_shared<Movie>(movieId, genre, views, vipOnly);
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    try {
        if (this->movies.insert(movieId, movie))
            return StatusType::FAILURE;

        this->moviesByGenre[static_cast<int>(genre)].insert(*movie, movie);
        this->moviesByGenre[static_cast<int>(Genre::NONE)].insert(*movie, movie);
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
    if (movieId <= 0)
        return StatusType::INVALID_INPUT;

    MovieNode* movieNode = this->movies.find(movieId, this->movies.getRoot());
    if (!movieNode)
        return StatusType::FAILURE;

    try {
        movieSharedPtr movie = movieNode->getValue();
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
    if (userId <= 0)
        return StatusType::INVALID_INPUT;

    userSharedPtr user;
    try {
        user = std::make_shared<User>(userId, isVip);
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    try {
        if (this->users.insert(userId, user)) {
            return StatusType::FAILURE;
        }
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
    if (userId <= 0)
        return StatusType::INVALID_INPUT;

    UserNode* userNode = this->users.find(userId, this->users.getRoot());
    if (!userNode)
        return StatusType::FAILURE;

    userSharedPtr user = userNode->getValue();
    try {
        if (user->isInGroup()) {
            groupSharedPtr group = user->getGroup();
            group->removeUserViews(user);
            group->removeUser(userId);
        }
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

    groupSharedPtr group;
    try {
        group = std::make_shared<Group>(groupId);
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    try {
        if (this->groups.insert(groupId, group)) {
            return StatusType::FAILURE;
        }
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_group(int groupId)
{
    if (groupId <= 0)
        return StatusType::INVALID_INPUT;

    GroupNode* groupNode= this->groups.find(groupId, this->groups.getRoot());
    if (groupNode == nullptr)
        return StatusType::FAILURE;
    else
        groupNode->getValue()->updateUsersBeforeDelete();

    try {
        if (!this->groups.remove(groupId))
            return StatusType::FAILURE;
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
    if (userId <= 0 || groupId <= 0)
        return StatusType::INVALID_INPUT;

    UserNode* userNode = this->users.find(userId, users.getRoot());
    if (userNode == nullptr || userNode->getValue()->isInGroup())
        return StatusType::FAILURE;

    GroupNode* groupNode = this->groups.find(groupId, groups.getRoot());
    if (groupNode == nullptr)
        return StatusType::FAILURE;

    userSharedPtr user = userNode->getValue();
    groupSharedPtr group = groupNode->getValue();
    try {
        group->updateViews(user->getViewsByGenre());
        group->insertUser(user);
        user->assignGroup(group->getId(), group->getCounterByGenre(), groupNode->getValue());
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
    if (userId <= 0 || movieId <= 0)
        return StatusType::INVALID_INPUT;

    UserNode* userNode = this->users.find(userId, users.getRoot());
    if (userNode == nullptr)
        return StatusType::FAILURE;

    MovieNode* movieNode = this->movies.find(movieId, movies.getRoot());
    if (movieNode == nullptr || (movieNode->getValue()->isVipOnly() && !userNode->getValue()->isVipUser()))
        return StatusType::FAILURE;

    userSharedPtr user = userNode->getValue();
    movieSharedPtr movie = movieNode->getValue();
    moviesByGenre[static_cast<int>(movie->getGenre())].remove(*movie);
    moviesByGenre[static_cast<int>(Genre::NONE)].remove(*movie);
    user->watchMovie(movieNode->getValue()->getGenre());
    movieNode->getValue()->updateViews(1);
    if (user->isInGroup()) {
        groupSharedPtr group = user->getGroup();
        group->soloWatch(movieNode->getValue()->getGenre());
    }

    moviesByGenre[static_cast<int>(movie->getGenre())].insert(*movie, movie);
    moviesByGenre[static_cast<int>(Genre::NONE)].insert(*movie, movie);

    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
    if (groupId <= 0 || movieId <= 0)
        return StatusType::INVALID_INPUT;

    GroupNode* groupNode = this->groups.find(groupId, groups.getRoot());
    if (groupNode == nullptr || groupNode->getValue()->getSize() == 0)
        return StatusType::FAILURE;

    MovieNode* movieNode = this->movies.find(movieId, movies.getRoot());
    if (movieNode == nullptr || (movieNode->getValue()->isVipOnly() && !groupNode->getValue()->isVipGroup()))
        return StatusType::FAILURE;

    //watch movie
    movieSharedPtr movie = movieNode->getValue();
    groupSharedPtr group = groupNode->getValue();

    moviesByGenre[static_cast<int>(movie->getGenre())].remove(*movie);
    moviesByGenre[static_cast<int>(Genre::NONE)].remove(*movie);

    group->groupWatch(movie->getGenre());
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
    Node<Movie, movieSharedPtr>* root = moviesByGenre[static_cast<int>(genre)].getRoot();
    moviesByGenre[static_cast<int>(genre)].inOrder(root, output, pos);

    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
    if (userId <= 0)
        return StatusType::INVALID_INPUT;

    UserNode* userNode = this->users.find(userId, users.getRoot());
    if (userNode == nullptr)
        return StatusType::FAILURE;

    int views = 0;
    userSharedPtr user = userNode->getValue();

    if (genre == Genre::NONE)
    {
        views += user->getViews();
        if (user->isInGroup()){
            groupSharedPtr group = user->getGroup();
            views += group->getTotalGroupViews() - user->getTotalGroupViewsBeforeJoined();
        }
    }
    else
    {
        views += user->getViewsByGenre(genre);
        if (user->isInGroup()) {
            groupSharedPtr group = user->getGroup();
            int groupCounterByGenre = group->getCounterByGenre(genre);
            int groupCounterBeforeJoined = user->getGroupCounterBeforeJoined(genre);

            if (groupCounterByGenre - groupCounterBeforeJoined > 0)
                views += groupCounterByGenre - groupCounterBeforeJoined;
        }
    }
    output_t<int> output(views);
    return output;
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
    if (userId <= 0 || movieId <= 0 || rating < 0 || rating > 100)
        return StatusType::INVALID_INPUT;

    UserNode* userNode = this->users.find(userId, users.getRoot());
    if (userNode == nullptr)
        return StatusType::FAILURE;

    MovieNode* movieNode = this->movies.find(movieId, movies.getRoot());
    if (movieNode == nullptr || (movieNode->getValue()->isVipOnly() && !userNode->getValue()->isVipUser()))
        return StatusType::FAILURE;
    //---------------------------------------------

    movieSharedPtr movie = movieNode->getValue();
    moviesByGenre[static_cast<int>(movie->getGenre())].remove(*movie);
    moviesByGenre[static_cast<int>(Genre::NONE)].remove(*movie);
    movie->updateRating(rating);
    moviesByGenre[static_cast<int>(movie->getGenre())].insert(*movie, movie);
    moviesByGenre[static_cast<int>(Genre::NONE)].insert(*movie, movie);

    //---------------------------------------------
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
	if (groupId <= 0)
        return StatusType::INVALID_INPUT;

    GroupNode* groupNode = this->groups.find(groupId, groups.getRoot());

    if (groupNode == nullptr || groupNode->getValue()->getSize() == 0)
        return StatusType::FAILURE;

    Genre favoriteGenre = groupNode->getValue()->getFavoriteGenre();
    if (favoriteGenre == Genre::NONE || this->moviesByGenre[static_cast<int>(favoriteGenre)].getSize() == 0)
        return StatusType::FAILURE;

    movieSharedPtr movie = this->moviesByGenre[static_cast<int>(favoriteGenre)].getMinNodeValue();
    return movie->getId();
}

