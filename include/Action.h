#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include"../include/User.h"
using namespace std;

class Session;


enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
    BaseAction();
    virtual ~BaseAction() = default;
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
    virtual string enumToString(ActionStatus type) const;
    virtual std::string getErrorMsg() const;
    virtual BaseAction* copyAction();
protected:
    void complete();
    void error(const std::string& errorMsg);
private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    CreateUser(string name, string algo); // construcrot
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual std::string getErrorMsg() const;
    virtual BaseAction* copyAction();
//    ~CreateUser();
private:
    string name; //new fields
    string algo; //new fields
//    User *user;
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser(string name); // constructor
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual std::string getErrorMsg() const;
    virtual BaseAction* copyAction();
private:
    string name; // new field
};

class DeleteUser : public BaseAction {
public:
    DeleteUser (string name); //constructor
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual std::string getErrorMsg() const;
    virtual BaseAction* copyAction();
private:
    string toDelete; // new field
};


class DuplicateUser : public BaseAction {
public:
    DuplicateUser(string toDup,string newName);
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual std::string getErrorMsg() const;
    virtual BaseAction* copyAction();
private:
    string toDup,newName;
};

class PrintContentList : public BaseAction {
public:
    PrintContentList(); // constructor
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual std::string getErrorMsg() const;
    virtual BaseAction* copyAction();
};

class PrintWatchHistory : public BaseAction {
public:
    PrintWatchHistory();
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual std::string getErrorMsg() const;
    virtual BaseAction* copyAction();
};


class Watch : public BaseAction {
public:
    Watch(long id);
    long getID();
    void setID(long newID);
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual std::string getErrorMsg() const;
    virtual BaseAction* copyAction();
private:
    long id;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual std::string getErrorMsg() const;
    virtual BaseAction* copyAction();
};

class Exit : public BaseAction {
public:
    Exit();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual std::string getErrorMsg() const;
    virtual BaseAction* copyAction();
};
#endif