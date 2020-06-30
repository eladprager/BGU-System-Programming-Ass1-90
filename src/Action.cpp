//
// Created by kananleo@wincs.cs.bgu.ac.il on 25/11/2019.
//

#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
using namespace std;

BaseAction::BaseAction() {
    status = PENDING ,errorMsg = "";
}
CreateUser::CreateUser(string name,string algo): name(name),algo(algo){}
ChangeActiveUser::ChangeActiveUser(string name): name(name){}
DeleteUser::DeleteUser(string name): toDelete(name){}
DuplicateUser::DuplicateUser(string toDup, string newName): toDup(toDup),newName(newName) {}
PrintContentList::PrintContentList() {}
PrintWatchHistory::PrintWatchHistory(){}
PrintActionsLog::PrintActionsLog(){}
Watch::Watch(long id): id(id){}
Exit::Exit() {}


ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

string BaseAction::enumToString(ActionStatus type) const {
    switch(type) {
    case COMPLETED: return "COMPLETED";
    case PENDING: return "PENDING";
    case ERROR: return "ERROR";
    }
    return nullptr;
}

BaseAction *BaseAction::copyAction() {
    return nullptr;
}

void CreateUser::act(Session &sess) {
    if (sess.getMap().count(this->name)){
        error(": name is taken");
        cout << "ERROR: name is taken" << endl;
    }
    else if (this->algo != "len" && this->algo != "rer" && this->algo != "gen") {
        error(": letter code is invalid");
        cout << "ERROR: letter code is invalid" << endl;
    }
    else {
        if (algo == "len") {
            LengthRecommenderUser *u1 = new LengthRecommenderUser(name);
            sess.addToMap(make_pair(name,u1));
            complete();
        }
        if (algo == "rer") {
            RerunRecommenderUser *u2 = new RerunRecommenderUser(name);
            sess.addToMap(make_pair(name,u2));
            complete();
        }
        if (algo == "gen") {
            GenreRecommenderUser *u3 = new GenreRecommenderUser(name);
            sess.addToMap(make_pair(name,u3));
            complete();
        }
    }
}

std::string CreateUser::toString() const {
    return "CreateUser "+ enumToString(getStatus());
}

std::string CreateUser::getErrorMsg() const {
    return BaseAction::getErrorMsg();
}

BaseAction *CreateUser::copyAction() {
    CreateUser* toReturn = new CreateUser(this->name,this->algo);
    if (this->getStatus() == ERROR) toReturn->error(this->getErrorMsg());
    if (this->getStatus() == COMPLETED) toReturn->complete();
    return toReturn;
}


void ChangeActiveUser::act(Session &sess) {
    if (sess.getMap().count(this->name) == false) {
        error(": no such user");
        cout << "ERROR: no such user" << endl;
    }
    else {
        User* use;
        for (pair<string,User*> X : sess.getMap()){
            if (X.first == this->name)
                use = X.second;
        }
        sess.setActiveUser(use);
        complete();
    }
}

std::string ChangeActiveUser::toString() const {
    return "ChangeActiveUser "+ enumToString(getStatus());
}

std::string ChangeActiveUser::getErrorMsg() const {
    return BaseAction::getErrorMsg();
}

BaseAction *ChangeActiveUser::copyAction() {
    ChangeActiveUser* toReturn = new ChangeActiveUser(this->name);
    if (this->getStatus() == ERROR) toReturn->error(this->getErrorMsg());
    if (this->getStatus() == COMPLETED) toReturn->complete();
    return toReturn;
}

void DeleteUser::act(Session &sess) {
    if (sess.getMap().count(toDelete) == false) {
        error(": no such user");
        cout << "ERROR: no such user" << endl;
    }
    else {
        for (pair<string,User*> X: sess.getMap()){
            if (X.first == toDelete) {
                //delete X.second;
                //X.second = nullptr;
                sess.deleteFromMap(X.first);
                break;
            }
        }
        complete();
    }
}

std::string DeleteUser::toString() const {
    return "DeleteUser " + enumToString(getStatus());
}

std::string DeleteUser::getErrorMsg() const {
    return BaseAction::getErrorMsg();
}

BaseAction *DeleteUser::copyAction() {
    DeleteUser* toReturn = new DeleteUser(this->toDelete);
    if (this->getStatus() == ERROR) toReturn->error(this->getErrorMsg());
    if (this->getStatus() == COMPLETED) toReturn->complete();
    return toReturn;
}

void DuplicateUser::act(Session &sess) {
    if(!sess.getMap().count(this->toDup)) {
        error(": original user doesn't exist");
        cout << "ERROR: original user doesn't exist" << endl;
    }
    if(sess.getMap().count(this->newName)) {
        error(": new user name is already taken");
        cout << "ERROR: new user name is already taken" << endl;
    }
    User* use;
    for (pair<string,User*> X : sess.getMap()){
        if (X.first == this->toDup)
            use = X.second->CopyUser();
    }

    CreateUser newUser = CreateUser(newName,use->getAlgo());
    newUser.act(sess);
    for (pair<string,User*> X : sess.getMap()){
        if (X.first == this->newName) {
            X.second->setWatchHistory(use->get_history());
            X.second->copyToWatch(use->getToWatch());
            delete use;
            break;
        }
    }
    complete();

}

std::string DuplicateUser::toString() const {
    return "DuplicateUser " + enumToString(getStatus());
}

std::string DuplicateUser::getErrorMsg() const {
    return BaseAction::getErrorMsg();
}

BaseAction *DuplicateUser::copyAction() {
    DuplicateUser* toReturn = new DuplicateUser(this->toDup,this->newName);
    if (this->getStatus() == ERROR) toReturn->error(this->getErrorMsg());
    if (this->getStatus() == COMPLETED) toReturn->complete();
    return toReturn;
}

void PrintContentList::act(Session &sess) {
    for (size_t i = 0; i < sess.getContent().size(); i++) {
        cout << to_string(i + 1) << "." <<  sess.getContent()[i]->toString()<< " " << to_string(sess.getContent()[i]->getLength()) << " minutes " << "[" << sess.getContent()[i]->getTags() << "]" <<  endl;
    }
    complete();
}

std::string PrintContentList::toString() const {
    return "PrintContentList "+ enumToString(getStatus());
}

std::string PrintContentList::getErrorMsg() const {
    return BaseAction::getErrorMsg();
}

BaseAction *PrintContentList::copyAction() {
    PrintContentList* toReturn = new PrintContentList();
    if (this->getStatus() == ERROR) toReturn->error(this->getErrorMsg());
    if (this->getStatus() == COMPLETED) toReturn->complete();
    return toReturn;
}


void PrintWatchHistory::act(Session &sess) {
    cout << "Watch history for " + sess.getActiveUser()->getName() << endl;
    for (size_t i = 0; i < sess.getActiveUser()->get_history().size(); i++){
        cout << i+1 << ". " << sess.getActiveUser()->get_history()[i]->toString() << endl;
    }
    complete();
}

std::string PrintWatchHistory::toString() const {
    return "PrintWatchHistory "+ enumToString(getStatus());
}

std::string PrintWatchHistory::getErrorMsg() const {
    return BaseAction::getErrorMsg();
}

BaseAction *PrintWatchHistory::copyAction() {
    PrintWatchHistory* toReturn = new PrintWatchHistory();
    if (this->getStatus() == ERROR) toReturn->error(this->getErrorMsg());
    if (this->getStatus() == COMPLETED) toReturn->complete();
    return toReturn;
}

void Watch::act(Session &sess) {
    cout <<"Watching " + sess.getContent()[id-1]->toString() << endl;
    sess.getActiveUser()->addToHistory(sess.getContent()[id-1]->copyWatch());
    Watchable* recommend = sess.getActiveUser()->get_history()[sess.getActiveUser()->get_history().size()-1]->getNextWatchable(sess);
    string toPrint = recommend->toString();
    cout << "We recommend watching " + toPrint << ", continue watching? [y/n]";
    long id = recommend->getID()+1;
    setID(id);
    string input;
    cin >> input;
    if (input=="yes") {
        act(sess);
    }
    complete();
}

std::string Watch::toString() const {
    return "Watch "+ enumToString(getStatus());
}

long Watch::getID() {
    return id;
}

void Watch::setID(long newID) {
    this->id=newID;
}

std::string Watch::getErrorMsg() const {
    return BaseAction::getErrorMsg();
}

BaseAction *Watch::copyAction() {
    Watch* toReturn = new Watch(this->id);
    if (this->getStatus() == ERROR) toReturn->error(this->getErrorMsg());
    if (this->getStatus() == COMPLETED) toReturn->complete();
    return toReturn;
}

void PrintActionsLog::act(Session &sess) {
    string actionlog = "";
    for (int i = sess.getActionLog().size()-1; i >= 0; i--){
        actionlog = actionlog + sess.getActionLog()[i]->toString() + sess.getActionLog()[i]->getErrorMsg() + "\n";
    }
    cout << actionlog;
    complete();
}

std::string PrintActionsLog::toString() const {
    return "PrintActionsLog "+ enumToString(getStatus());
}

std::string PrintActionsLog::getErrorMsg() const {
    return BaseAction::getErrorMsg();
}

BaseAction *PrintActionsLog::copyAction() {
    PrintActionsLog* toReturn = new PrintActionsLog();
    if (this->getStatus() == ERROR) toReturn->error(this->getErrorMsg());
    if (this->getStatus() == COMPLETED) toReturn->complete();
    return toReturn;
}

void Exit::act(Session &sess) {
    complete();
}

std::string Exit::toString() const {
    return "Exit "+ enumToString(getStatus());
}

std::string Exit::getErrorMsg() const {
    return BaseAction::getErrorMsg();
}

BaseAction *Exit::copyAction() {;
    Exit* toReturn = new Exit();
    if (this->getStatus() == ERROR) toReturn->error(this->getErrorMsg());
    if (this->getStatus() == COMPLETED) toReturn->complete();
    return toReturn;
}

