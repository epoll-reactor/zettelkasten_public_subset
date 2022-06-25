#ifndef SUB_NOT_PATTERN_HPP
#define SUB_NOT_PATTERN_HPP

#include <bits/stdc++.h>

class Subscriber;

class Notifier
{
public:
    using subscriber_id_t = std::size_t;
    using subscriber_t = std::pair<std::shared_ptr<Subscriber>, std::function<void()>>;
    void notify_all();
    void remove_subscriber(std::size_t subscriber_id);

private:
    friend class Subscriber;

    subscriber_id_t add_subscriber(std::shared_ptr<Subscriber> subscriber, std::function<void()> callback);
    std::unordered_map<std::size_t, subscriber_t> m_subscribers;
};

class Subscriber : public std::enable_shared_from_this<Subscriber>
{
public:
    Notifier::subscriber_id_t subscribe(Notifier& notifier, std::function<void()> on_notify_callback)
    {
        m_on_notify_callback = std::move(on_notify_callback);
        return notifier.add_subscriber(shared_from_this(), m_on_notify_callback);
    }

    void on_notify() noexcept
    {
        m_on_notify_callback();
    }

private:
    std::function<void()> m_on_notify_callback;
};

Notifier::subscriber_id_t Notifier::add_subscriber(std::shared_ptr<Subscriber> subscriber, std::function<void()> callback)
{
    static subscriber_id_t subscribers_count = 0;
    m_subscribers[subscribers_count] = { subscriber, std::move(callback) };
    return subscribers_count++;
}

void Notifier::remove_subscriber(std::size_t subscriber_id)
{
    if (std::size_t elements_erased = m_subscribers.erase(subscriber_id); elements_erased == 0)
    {
        throw std::runtime_error("No such subscriber");
    }
}

void Notifier::notify_all()
{
    for (auto&& subscriber : m_subscribers)
    {
        auto& sub_object = subscriber.second.first;
        sub_object->on_notify();
    }
}

void run()
{
    Notifier notifier;

    [[maybe_unused]] std::size_t id1 = std::make_shared<Subscriber>()->subscribe(notifier, [](){ std::cout << "callback 1" << std::endl; });
    [[maybe_unused]] std::size_t id2 = std::make_shared<Subscriber>()->subscribe(notifier, [](){ std::cout << "callback 2" << std::endl; });
    [[maybe_unused]] std::size_t id3 = std::make_shared<Subscriber>()->subscribe(notifier, [](){ std::cout << "callback 3" << std::endl; });
    [[maybe_unused]] std::size_t id4 = std::make_shared<Subscriber>()->subscribe(notifier, [](){ std::cout << "callback 4" << std::endl; });

    notifier.notify_all();
}

#endif // SUB_NOT_PATTERN_HPP
