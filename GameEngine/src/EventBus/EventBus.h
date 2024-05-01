#pragma once

#include"../logger/Logger.h"
#include <map>
#include <vector>
#include <typeindex>
#include <memory>
#include <list>
#include "Event.h"


class IEventCallback {
	private:
		//pure virtual 
		virtual void Call(Event& e) = 0;

	public:

		virtual ~IEventCallback() = default;

		void Execute(Event& e) {
			Call(e);
		}

};

template <typename TOwner, typename TEvent>
class EventCallback: public IEventCallback {
	private:
		//bruh what is happening here
		typedef void (TOwner::* CallbackFuncion)(TEvent&);

		TOwner* ownerInstance;
		CallbackFuncion callbackFunction;

		virtual void Call(Event& e) override 
		{
			std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
		}
	public:
		//CallbackFunction is defined above as a pointer to a function so its already a pointer
		EventCallback(TOwner* ownerInstance, CallbackFuncion callbackFunction) {
			this->ownerInstance = ownerInstance;
			this->callbackFunction = callbackFunction;
		}

		virtual ~EventCallback() override = default;
};

//Since our EvenCallback is a template class we would need to pass it a specific
//Template type but not if we inherit from another class and pass it in as the 
//type of the unique pointer
typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {

	private:

		std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

	public:
		EventBus() {
			Logger::Log("Event Bus initiated");
		}

		~EventBus() {
			Logger::Log("Event Bus deleted");

		}

		/// <summary>
		/// Subscribe to an event type <T>
		/// In the implementation a lisitener subscribes to an event as so
		/// Example: eventBus->SubscribeToEvent<CollisionEvent>(this, &Game:onCollision);
		/// </summary>
		template<typename TEvent, typename TOwner>
		void SubscribeToEvent(TOwner* ownerInstance, void(TOwner::* callbackFunction)(TEvent&)) {
			

			//if the subscribers map at the TEvent type has no HandlerList
			//create a new handler list
			if (subscribers[typeid(TEvent)].get() == nullptr) 
			{
				subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
			}

			//Here we create a unique pointer of type EvenCallback which is a template class, taking
			//in the TOwner and TEvent types deduced in this function, with the constructor passing in
			//a pointer to the ownerInstance and a callback function which is a pointer function
			//could use auto but I like this more vurbose better to actually know what the fuck is going on
			std::unique_ptr<EventCallback<TOwner,TEvent>> subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);


			//Since our subscriber is a unique pointer, we need to use std::move when we pushing to the map.
			//Using std::move changes the ownership of an object from one unique_ptr to another unique_ptr
			subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
		}
		//TOOD: 
	
		/// <summary>
		/// Emit an event type <T>
		/// In the implementation a system has emit an event as so
		/// Example: eventBus->EmitEvent<CollisionEvent>(Argument a, Argument b);
		/// </summary>
		template <typename TEvent, typename ...TArgs>
		void EmitEvent(TArgs&& ...args)
		{
			auto handlers = subscribers[typeid(TEvent)].get();
			if (handlers)
			{
				for (auto it = handlers->begin(); it != handlers->end(); it++)
				{
					auto handler = it->get();

					//bruh TEvent is not real until it is called, shrodinger's type
					TEvent event(std::forward<TArgs>(args)...);
					
					//what is happening
					handler->Execute(event);
				}
			}
		}
};