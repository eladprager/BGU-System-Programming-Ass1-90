
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <utility>
using namespace std;

User::User(const std::string &name): name(name) {
}

std::string User::getName()const {
    return name;
}

std::vector<Watchable *> User::get_history() const {
    return history;
}

void User::addToHistory(Watchable* u) {
    for (size_t i = 0; i < toWatch.size(); i++){
        if (toWatch[i]->getID() == u->getID()){
            delete toWatch[i];
            toWatch[i]= nullptr;
            toWatch.erase(toWatch.begin()+i);
        }
    }
    history.push_back(u);
}

string User::getAlgo() {
    return algo;
}

void User::setWatchHistory(vector<Watchable *> toCopy) {
    for (size_t i = 0; i < toCopy.size(); i++){
        history.push_back(toCopy[i]->copyWatch());
    }
}

vector<Watchable *> User::getToWatch() {
    return this->toWatch;
}

void User::addToWatch(Watchable *toAdd) {
    this->toWatch.push_back(toAdd);
}

void User::delFromWatch(int index) {
    delete toWatch[index];
    toWatch[index] = nullptr;
    toWatch.erase(toWatch.begin()+index);
}

void User::copyToWatch(vector<Watchable *> toCopy) {
    for (size_t i = 0; i < toCopy.size(); i++){
        toWatch.push_back(toCopy[i]->copyWatch());
    }
}
User::~User() {
      for (size_t i=0; i<toWatch.size();i++) {
        delete toWatch[i];
        toWatch[i] = nullptr;
    }
    toWatch.clear();

    for (size_t i=0; i<history.size();i++) {
        delete history[i];
        history[i] = nullptr;
    }
    history.clear();
}

User *User::CopyUser() {
    return nullptr;
}

LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name){
    algo = "len";
}

Watchable *LengthRecommenderUser::getRecommendation(Session &s) {
    double avgLength = 0;
    for (size_t i = 0; i < s.getActiveUser()->get_history().size(); i++) {
        avgLength = avgLength + s.getActiveUser()->get_history()[i]->getLength();
    }
    avgLength = avgLength / (s.getActiveUser()->get_history().size());
    if (history.size() == 1){
        long index = history[0]->getID();
        for (size_t i = 0; i < s.getContent().size(); i++){
            s.getActiveUser()->addToWatch(s.getContent()[i]->copyWatch());
        }
        s.getActiveUser()->delFromWatch(index);
    }
    Watchable *toRecomend;
    toRecomend = s.getActiveUser()->getToWatch()[0];
    int minDelta = abs(avgLength-s.getActiveUser()->getToWatch()[0]->getLength());
    for (size_t i = 1; i < s.getActiveUser()->getToWatch().size(); i++){
        if(abs(avgLength-s.getActiveUser()->getToWatch()[i]->getLength()) < minDelta){
            minDelta = abs(avgLength-s.getActiveUser()->getToWatch()[i]->getLength());
            toRecomend = s.getActiveUser()->getToWatch()[i];
        }
    }
    return toRecomend;
}

string LengthRecommenderUser::getAlgo() {
    return algo;
}

User *LengthRecommenderUser::CopyUser() {
    LengthRecommenderUser* toReturn = new LengthRecommenderUser(this->getName());
    toReturn->copyToWatch(this->getToWatch());
    toReturn->setWatchHistory(this->history);
    return toReturn;
}

RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name) {
    algo = "rer";
    index = 0;
}

Watchable *RerunRecommenderUser::getRecommendation(Session &s) {
    if (history.size() == 1){
        long index = history[0]->getID();
        for (size_t i = 0; i < s.getContent().size(); i++){
            s.getActiveUser()->addToWatch(s.getContent()[i]->copyWatch());
        }
        s.getActiveUser()->delFromWatch(index);
    }
    Watchable *toRecomend;
    int n = s.getActiveUser()->get_history().size();
    toRecomend = s.getActiveUser()->get_history()[(index+1)%n];
    index = (index+1)%n;
    return toRecomend;
}

string RerunRecommenderUser::getAlgo() {
    return algo;
}

User *RerunRecommenderUser::CopyUser() {
    RerunRecommenderUser *toReturn = new RerunRecommenderUser(this->getName());
    toReturn->copyToWatch(this->getToWatch());
    toReturn->setWatchHistory(this->history);
    toReturn->index = this->index;
    return toReturn;
}

GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name) {
    algo = "gen";
}

bool cmp(const pair<string,int> &p1,const pair<string,int> &p2)
{
    if(p1.second!=p2.second)
        return p1.second > p2.second;
    return (p1.first < p2.first);
}

Watchable *GenreRecommenderUser::getRecommendation(Session &s) {
    if (history.size() == 1){
        long index = history[0]->getID();
        for (size_t i = 0; i < s.getContent().size(); i++){
            s.getActiveUser()->addToWatch(s.getContent()[i]->copyWatch());
        }
        s.getActiveUser()->delFromWatch(index);
    }
    Watchable *toRecomend;
    vector<pair<string,int>> tagsVectorByPop;
    for (size_t i = 0; i < s.getActiveUser()->get_history().size(); i++){
        for (size_t j = 0; j < s.getActiveUser()->get_history()[i]->getTagsVector().size(); j++){
            bool notTag = false;
            for (size_t k = 0; k < tagsVectorByPop.size(); k++) {
                if (tagsVectorByPop[k].first == s.getActiveUser()->get_history()[i]->getTagsVector()[j]) {
                    tagsVectorByPop[k].second++;
                    notTag = true;
                    break;
                }
            }
            if (notTag != true) {
                tagsVectorByPop.push_back(make_pair(s.getActiveUser()->get_history()[i]->getTagsVector()[j],1));
            }
        }
    }
    sort(tagsVectorByPop.begin(),tagsVectorByPop.end(),cmp);

    for (size_t k = 0; k < tagsVectorByPop.size(); k++) {
        for (size_t i = 0; i < s.getActiveUser()->getToWatch().size(); i++) {
            for (size_t j = 0; j < s.getActiveUser()->getToWatch()[i]->getTagsVector().size(); j++) {
                if (s.getActiveUser()->getToWatch()[i]->getTagsVector()[j] == tagsVectorByPop[k].first) {
                    toRecomend = s.getActiveUser()->getToWatch()[i];
                    return toRecomend;
                }
            }
        }
    }
    return nullptr;
}

string GenreRecommenderUser::getAlgo() {
    return algo;
}

User *GenreRecommenderUser::CopyUser() {
    GenreRecommenderUser *toReturn = new GenreRecommenderUser(this->getName());
    toReturn->copyToWatch(this->getToWatch());
    toReturn->setWatchHistory(this->history);
    return toReturn;
}
