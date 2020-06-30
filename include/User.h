#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
using namespace std;
class Watchable;
class Session;

class User{
public:
//    bool cmp(const pair<string,int> &p1,const pair<string,int> &p2);
    User(const std::string& name);
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    void addToHistory(Watchable* u);
    void setWatchHistory(vector<Watchable*> toCopy);
    virtual string getAlgo() = 0;
    vector <Watchable*> getToWatch();
    void addToWatch(Watchable* toAdd);
    void delFromWatch(int index);
    void copyToWatch(vector<Watchable*> toCopy);
    virtual User* CopyUser();
    virtual ~User();
protected:
    std::vector<Watchable*> history;
private:
    vector<Watchable*> toWatch;
    std::string name;
    string algo;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual string getAlgo();
    virtual User* CopyUser();
private:
    string algo;
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual string getAlgo();
    virtual User* CopyUser();
private:
    string algo;
    int index;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual string getAlgo();
    virtual User* CopyUser();
private:
    string algo;
};

#endif
