//
// Created by kananleo@wincs.cs.bgu.ac.il on 24/11/2019.
//

#include "../include/Watchable.h"
#include "../include/Session.h"
using namespace std;


Watchable::Watchable(long id, int length, const std::vector<std::string>& tags):id(id),length(length),tags(tags)  {}


Watchable::~Watchable() {

}

int Watchable::getLength() {
    return length;
}

long Watchable::getID() {
    return id;
}

string Watchable::getTags() {
    string toReturn = tags[0].substr(1, tags[0].size() - 2);;
    for(size_t i=1; i<tags.size();i++) {
        toReturn = toReturn + ", " + tags[i].substr(1, tags[i].size() - 2);
    }
    return toReturn;
}

vector<string> Watchable::getTagsVector() {
    return this->tags;
}

Watchable *Watchable::copyWatch() {
    return nullptr;
}


Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags),name(name) {}

std::string Movie::toString() const {
    string str = name;
    return str;
}

Watchable *Movie::getNextWatchable(Session& sess) const {
    return sess.getActiveUser()->getRecommendation(sess);
}

std::string Movie::getName() {
    return this->name;
}

Watchable *Movie::copyWatch() {
    vector<string> k;
    for (size_t i = 0; i < this->getTagsVector().size(); i++){
        k.push_back(this->getTagsVector()[i]);
    }
    return new Movie(this->getID(),this->name,this->getLength(),k);
}

Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags): Watchable(id,length,tags),seriesName(seriesName),season(season),episode(episode) {
    this->nextEpisodeId = id+1;
}


std::string Episode::toString() const {
    string s = to_string(season);
    string e = to_string(episode);
    string n = seriesName;
    string ret = n+" S"+s+" E"+e;
    return ret;
}

Watchable *Episode::getNextWatchable(Session& sess) const {
    Watchable* recommend;
    if (sess.getActiveUser()->get_history().size() == 1){
        long index = sess.getActiveUser()->get_history()[0]->getID();
        for (size_t i = 0; i < sess.getContent().size(); i++){
            sess.getActiveUser()->addToWatch(sess.getContent()[i]->copyWatch());
        }
        sess.getActiveUser()->delFromWatch(index);
    }
    recommend = sess.getContent()[nextEpisodeId];
    return recommend;
}

std::string Episode::getName() {
    return this->seriesName;
}

Watchable *Episode::copyWatch() {
    vector<string> k;
    for (size_t i = 0; i < this->getTagsVector().size(); i++){
        k.push_back(this->getTagsVector()[i]);
    }
    return new Episode(this->getID(),this->seriesName,this->getLength(),this->season,this->episode,k);
}




