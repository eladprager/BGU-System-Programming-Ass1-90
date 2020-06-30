#ifndef SESSION_H_
#define SESSION_H_
using namespace std  ;
#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    Session();
    ~Session();
    Session(const Session &source);
    Session (Session&& other);
    Session& operator=(const Session& different);
    Session& operator=(Session&& different);
    void start();
    void addToMap(pair<string,User*> newpair);
    unordered_map<string,User*> getMap();
    vector<Watchable*> getContent();
    User* getActiveUser();
    void deleteFromMap(string name);
    void setActiveUser(User* user);
    vector<BaseAction*> getActionLog();
    void addToActionLog(BaseAction* base);

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
};
#endif
