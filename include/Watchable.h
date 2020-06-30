#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
using namespace std;

class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    int getLength();
    string getTags();
    vector<string> getTagsVector();
    long getID();
    virtual ~Watchable();
    virtual std::string getName() = 0;
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual Watchable* copyWatch();
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual std::string getName();
    virtual Watchable* getNextWatchable(Session& sess) const;
    virtual Watchable* copyWatch();
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual std::string getName();
    virtual Watchable* getNextWatchable(Session& sess) const;
    virtual Watchable* copyWatch();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
