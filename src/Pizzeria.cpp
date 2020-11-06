#include "Pizzeria.hpp"
#include <numeric>
#include <algorithm>
#include <thread>
#include <iostream>
#include <memory>


Pizzeria::Pizzeria(std::string const & name, std::unique_ptr<ITimer> timer)
    : name_(name) , orders_(), timer_ptr(std::move(timer))
{}

int Pizzeria::makeOrder(Pizzas pizzas)
{
    int orderId = rand() % 1000;
    orders_.push_back(std::make_tuple(orderId, pizzas, timer_ptr->now(), Status::New));
    return orderId;
}

double Pizzeria::calculatePrice(int orderId)
{
    auto order = std::find_if(orders_.begin(), orders_.end(), [orderId](const auto & order)
    {
        return std::get<0>(order) == orderId;
    });
    if (order != orders_.end())
    {
        std::get<Status>(*order) = Status::Paid;
        auto pizzas = std::get<Pizzas>(*order);
        return std::accumulate(pizzas.begin(),
                               pizzas.end(),
                               0.0,
                               [](double previousSum, Pizza *pizza)
                               {
                                   return previousSum + pizza->getPrice();
                               });
    }
    else {
    throw std::invalid_argument("Order with id: " + std::to_string(orderId) + "not found");
}
}

void Pizzeria::bakePizzas(int orderId)
{
    auto order = std::find_if(orders_.begin(), orders_.end(), [orderId](const auto & order)
    {
        return std::get<0>(order) == orderId;
    });
    if (order != orders_.end())
    {
        auto pizzas = std::get<Pizzas>(*order);
        for (const auto & pizza : pizzas)
        {
            std::cout << "Baking " << pizza->getName() << std::endl;
            timer_ptr->sleep_for(pizza->getBakingTime());
        }
        std::get<Status>(*order) = Status::Baked;
    }
    else {
    throw std::invalid_argument("Order with id: " + std::to_string(orderId) + "not found");
    }
}

void Pizzeria::completeOrder(int orderId)
{
    auto order = std::find_if(orders_.begin(), orders_.end(), [orderId](const auto & order)
    {
        return std::get<0>(order) == orderId;
    });
    if (order != orders_.end())
    {
        std::cout << "Order " << orderId << " completed" << std::endl;
        std::get<Status>(*order) = Status::Completed;
    }
    else {
    throw std::invalid_argument("Order with id: " + std::to_string(orderId) + "not found");
    }
}


