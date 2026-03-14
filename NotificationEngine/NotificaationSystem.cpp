#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// =================================================================
//                  Notification & Decorators
// =================================================================

// Interface for Notification
class INotification
{
public:
    virtual string getContent() = 0;
    virtual ~INotification() {}
};

// Concrete Notification class
class SimpleNotification : public INotification
{
private:
    string message;

public:
    SimpleNotification(string msg) : message(msg) {}

    string getContent() override
    {
        return message;
    }
};

// Base Decorator
class INotificationDecorator : public INotification
{
protected:
    INotification *notification;

public:
    INotificationDecorator(INotification *notif) : notification(notif) {}
    virtual ~INotificationDecorator()
    {
        delete notification;
    }
};

// Timestamp helper
string getCurrentTime()
{
    return "2024-06-01 12:00:00";
}

// Timestamp Decorator
class TimestampDecorator : public INotificationDecorator
{
public:
    TimestampDecorator(INotification *notif)
        : INotificationDecorator(notif) {}

    string getContent() override
    {
        return "[" + getCurrentTime() + "] " + notification->getContent();
    }
};

// Signature Decorator
class SignatureDecorator : public INotificationDecorator
{
private:
    string signature;

public:
    SignatureDecorator(INotification *notif, string sig)
        : INotificationDecorator(notif), signature(sig) {}

    string getContent() override
    {
        return notification->getContent() + "\n-- " + signature;
    }
};

// =================================================================
//                      Observer Pattern
// =================================================================

// Observer interface
class IObserver
{
public:
    virtual void update() = 0;
    virtual ~IObserver() {}
};

// Observable interface
class IObservable
{
public:
    virtual void addObserver(IObserver *observer) = 0;
    virtual void removeObserver(IObserver *observer) = 0;
    virtual void notifyObservers() = 0;
    virtual ~IObservable() {}
};

class NotificationObservable : public IObservable
{
private:
    INotification *current_notification = nullptr;
    vector<IObserver *> observers;

public:
    void addObserver(IObserver *observer) override
    {
        observers.push_back(observer);
    }

    void removeObserver(IObserver *observer) override
    {
        observers.erase(
            remove(observers.begin(), observers.end(), observer),
            observers.end());
    }

    void notifyObservers() override
    {
        for (auto observer : observers)
        {
            observer->update();
        }
    }

    void setNotification(INotification *notification)
    {
        if (current_notification)
        {
            delete current_notification;
        }
        current_notification = notification;
        notifyObservers();
    }

    string getNotificationContent()
    {
        return current_notification ? current_notification->getContent() : "";
    }

    ~NotificationObservable()
    {
        delete current_notification;
    }
};

// Concrete Observer: Logger
class Logger : public IObserver
{
private:
    NotificationObservable *observable;

public:
    Logger(NotificationObservable *obs) : observable(obs) {}

    void update() override
    {
        cout << "Logger: New notification received:\n"
             << observable->getNotificationContent() << endl;
    }
};

// =================================================================
//                      Strategy Pattern
// =================================================================

class INotificationStrategy
{
public:
    virtual void sendNotification(string content) = 0;
    virtual ~INotificationStrategy() {}
};

class EmailNotificationStrategy : public INotificationStrategy
{
private:
    string email;

public:
    EmailNotificationStrategy(string email) : email(email) {}

    void sendNotification(string content) override
    {
        cout << "Email sent to " << email << ":\n"
             << content << endl;
    }
};

class SMSNotificationStrategy : public INotificationStrategy
{
private:
    string phone;

public:
    SMSNotificationStrategy(string phone) : phone(phone) {}

    void sendNotification(string content) override
    {
        cout << "SMS sent to " << phone << ":\n"
             << content << endl;
    }
};

// Observer + Strategy consumer
class NotificationEngine : public IObserver
{
private:
    NotificationObservable *observable;
    vector<INotificationStrategy *> strategies;

public:
    NotificationEngine(NotificationObservable *obs) : observable(obs) {}

    void addStrategy(INotificationStrategy *strategy)
    {
        strategies.push_back(strategy);
    }

    void update() override
    {
        string content = observable->getNotificationContent();
        for (auto strategy : strategies)
        {
            strategy->sendNotification(content);
        }
    }

    ~NotificationEngine()
    {
        for (auto s : strategies)
            delete s;
    }
};

// =================================================================
//                      Singleton Service
// =================================================================

class NotificationService
{
private:
    static NotificationService *instance;
    NotificationObservable *observable;

    NotificationService()
    {
        observable = new NotificationObservable();
    }

public:
    static NotificationService *getInstance()
    {
        if (!instance)
            instance = new NotificationService();
        return instance;
    }

    NotificationObservable *getObservable()
    {
        return observable;
    }

    void sendNotification(INotification *notification)
    {
        observable->setNotification(notification);
    }

    ~NotificationService()
    {
        delete observable;
    }
};

NotificationService *NotificationService::instance = nullptr;

// =================================================================
//                              MAIN
// =================================================================

int main()
{
    NotificationService *service = NotificationService::getInstance();

    Logger *logger = new Logger(service->getObservable());
    service->getObservable()->addObserver(logger);

    NotificationEngine *engine =
        new NotificationEngine(service->getObservable());
    service->getObservable()->addObserver(engine);

    engine->addStrategy(new EmailNotificationStrategy("user@example.com"));
    engine->addStrategy(new SMSNotificationStrategy("123-456-7890"));

    INotification *notification =
        new SignatureDecorator(
            new TimestampDecorator(
                new SimpleNotification("This is a test notification")),
            "Notification System");

    service->sendNotification(notification);

    delete logger;
    delete engine;

    return 0;
}