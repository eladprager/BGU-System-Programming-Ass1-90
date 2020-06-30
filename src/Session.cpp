#include "../include/Session.h"
#include"../include/json.hpp"
#include "../include/Watchable.h"
#include "../include/Action.h"
#include "../include/User.h"
#include<fstream>
#include<sstream>
using namespace std;

Session::Session(const std::string &configFilePath){
// read a JSON file
using json = nlohmann::json;
std::ifstream i(configFilePath);
json j;
i >> j;
long id = 0;
for (size_t i = 0;i <j["movies"].size();i++){
    string name = j["movies"][i]["name"].dump();
    name = name.substr(1, name.size() - 2);
    //string &name1 = name;
    int len = stoi(j["movies"][i]["length"].dump());
    vector<string> tagsv;
    for (size_t k = 0; k < j["movies"][i]["tags"].size();k++) {
        tagsv.push_back(j["movies"][i]["tags"][k].dump());
    }
   // vector<string> &tagsvr = tagsv;
    Movie *mov = new Movie(id,name,len,tagsv);
    content.push_back(mov);
    id++;
}
for (size_t i = 0; i < j["tv_series"].size();i++){
    string name = j["tv_series"][i]["name"].dump();
    name = name.substr(1, name.size() - 2);
   // string &name1 = name;
    vector<string> tagsv;
    for (size_t k = 0; k < j["tv_series"][i]["tags"].size();k++) {
        tagsv.push_back(j["tv_series"][i]["tags"][k].dump());
    }
  //  vector<string> &tagsvr = tagsv;
    int len = stoi(j["tv_series"][i]["episode_length"].dump());
    for (size_t f = 0; f < j["tv_series"][i]["seasons"].size();f++){
    int season = f; //season0
        for (int m = 0; m < stoi(j["tv_series"][i]["seasons"][f].dump()); m++){
            Episode *epi = new Episode(id,name,len,season,m,tagsv);
            content.push_back(epi);
            id++;
        }
    }
}
}

Session::Session() {

}

Session::~Session() {
    for (size_t i=0; i<content.size();i++) {
        delete content[i];
        content[i] = nullptr;
    }
    content.clear();

    for (size_t i=0; i<actionsLog.size();i++) {
        delete actionsLog[i];
        actionsLog[i] = nullptr;
    }
    actionsLog.clear();

    activeUser=nullptr;

    for (auto X : userMap){
        delete X.second;
        X.second = nullptr;
    }
    userMap.clear();
}

unordered_map<string,User*> Session::getMap() {
    return this->userMap;
}
void Session::setActiveUser(User* user) {
    this->activeUser = user;
}

void Session::start() {
    cout << "SPLflix is now on!" << endl;
    if (this->userMap.size() == 0){
        string d = "default";
        string &def = d;
        LengthRecommenderUser *defuser = new LengthRecommenderUser(def);
        setActiveUser(defuser);
        userMap.insert(make_pair(d,defuser));
    }

    string first = "start";

    while (first != "exit") {
        string input;
        getline(cin, input);
        int space1 = input.find(" ");
        first = input.substr(0, space1);
        string restof1 = input.substr(space1 + 1, input.length());
        int space2 = restof1.find(" ");
        string second = restof1.substr(0, space2);
        string third = restof1.substr(space2 + 1, input.length());

        if (first == "createuser") {
            CreateUser *b = new CreateUser(second, third);
            b->act(*this);
            actionsLog.push_back(b);
        }
        if (first == "changeuser") {
            ChangeActiveUser *b = new ChangeActiveUser(second);
            b->act(*this);
            actionsLog.push_back(b);
        }
        if (first == "deleteuser") {
            DeleteUser *b = new DeleteUser(second);
            b->act(*this);
            actionsLog.push_back(b);
        }
        if (first == "dupuser") {
            DuplicateUser *b = new DuplicateUser(second, third);
            b->act(*this);
            actionsLog.push_back(b);
        }
        if (first == "content") {
            PrintContentList *b = new PrintContentList();
            b->act(*this);
            actionsLog.push_back(b);
        }
        if (first == "watchhist") {
            PrintWatchHistory *b = new PrintWatchHistory();
            b->act(*this);
            actionsLog.push_back(b);
        }
        if (first == "watch") {
            Watch *b = new Watch(stol(second));
            b->act(*this);
            actionsLog.push_back(b);
        }
        if (first == "log") {
            PrintActionsLog *b = new PrintActionsLog();
            b->act(*this);
            actionsLog.push_back(b);
        }
        if (first == "exit") {
            Exit *b = new Exit();
            actionsLog.push_back(b);
            b->act(*this);

        }
    }
}

vector<Watchable *> Session::getContent() {
    return this->content;
}

User *Session::getActiveUser() {
    return this->activeUser;
}


void Session::addToMap(pair<string, User *> newpair) {
    userMap.insert(newpair);
}

void Session::deleteFromMap(string name) {
    for (pair<string,User*> X: userMap){
        if (X.first == name){
            delete X.second;
            X.second = nullptr;
            userMap.erase(X.first);
            break;
        }
    }
}

vector<BaseAction *> Session::getActionLog() {
    return this->actionsLog;
}

void Session::addToActionLog(BaseAction * base) {
    actionsLog.push_back(base);
}

Session::Session(const Session &source) {
    for (size_t i = 0; i < source.content.size(); i++){
        Watchable *k = source.content[i]->copyWatch();
        this->content.push_back(k);
    }

    for (size_t i = 0; i < source.actionsLog.size(); i++){
        BaseAction *k = source.actionsLog[i]->copyAction();
        this->actionsLog.push_back(k);
    }

    for (pair<string,User*> X: source.userMap) {
        User *k = X.second->CopyUser();
        this->userMap.insert(make_pair(X.first, k));
    }
    for (pair<string,User*> X: userMap) {
        if (source.activeUser->getName() == X.first){
            this->activeUser = X.second;
        }
    }
}
Session::Session(Session&& other) {
    activeUser = other.activeUser;
    other.activeUser = nullptr;
    this->content = other.content;
    for (size_t i = 0; i < other.content.size(); i++){
        other.content[i] = nullptr;
    }
    this->actionsLog = other.actionsLog;
    for (size_t i = 0; i < other.actionsLog.size(); i++){
        other.actionsLog[i] = nullptr;
    }
    this->userMap = other.userMap;
    for (auto X : other.userMap){
        X.second = nullptr;
    }
}

Session &Session::operator=(Session &&different) {
    if (this != &different){
        this->activeUser = nullptr;
        for (size_t i = 0; i < this->content.size(); i++){
            delete content[i];
            content[i] = nullptr;
        }
        content.clear();
        for (size_t i = 0; i < actionsLog.size(); i++){
            delete actionsLog[i];
            actionsLog[i] = nullptr;
        }
        actionsLog.clear();
        for (auto X : this->userMap){
            delete X.second;
            X.second = nullptr;
        }
        this->userMap.clear();
        this->activeUser = different.activeUser;
        this->content = different.getContent();
        this->actionsLog = different.getActionLog();
        this->userMap = different.getMap();
        for (size_t i = 0; i < different.getContent().size(); i++){
            different.getContent()[i] = nullptr;
        }
        for (size_t i = 0; i < different.getActionLog().size(); i++){
            different.getActionLog()[i] = nullptr;
        }
        for (auto X : different.getMap()){
            X.second = nullptr;
        }
    }
    return *this;
}

Session &Session::operator=(const Session &different) {
    if (&different != this){
        this->activeUser = nullptr;
        for (size_t i = 0; i < this->content.size(); i++){
            delete content[i];
            content[i] = nullptr;
        }
        content.clear();
        for (size_t i = 0; i < actionsLog.size(); i++){
            delete actionsLog[i];
            actionsLog[i] = nullptr;
        }
        actionsLog.clear();
        for (auto X : this->userMap){
            delete X.second;
            X.second = nullptr;
        }
        userMap.clear();
        for (auto X : different.userMap){
            User* user = X.second->CopyUser();
            userMap.insert(make_pair(X.first,user));
        }
        for (size_t i = 0; i < different.actionsLog.size(); i++){
            actionsLog.push_back(different.actionsLog[i]->copyAction());
        }
        for (size_t i = 0; i < different.content.size(); i++){
            content.push_back(different.content[i]->copyWatch());
        }
        for (auto X : userMap){
            if (X.first == different.activeUser->getName()){
                activeUser = X.second;
            }
        }
        return *this;
    }
    return *this;
}







