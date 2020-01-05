#include <PlaygroundApplication.hpp>

#include <iostream>
#include <fstream>
#include <Meatball/Core.hpp>
#include <Meatball/Core/ApplicationBuilder.hpp>
#include <Meatball/Windowing/ScriptedTestWindow.hpp>

using EventType = Meatball::Events::EventType;

PlaygroundApplication::PlaygroundApplication(Meatball::Unique<Meatball::Events::EventBus> eventBus)
		: Application(std::move(eventBus), std::make_unique<Meatball::Windowing::ScriptedTestWindow>(eventBus->GetDispatcher())) {

}

PlaygroundApplication::~PlaygroundApplication() {
}

class LoggingEventBus : public Meatball::Events::EventBus {
public:
	LoggingEventBus() {
		os.open("eventLog.log");
	}

	virtual void PublishEvent(Meatball::Shared<Meatball::Events::Event> event) override {
		os << event->GetName() << "\n";
		EventBus::PublishEvent(event);
	}

private: 
	std::ofstream os;
};


Meatball::Unique<Meatball::Application> Meatball::CreateApplication() {
	Meatball::ApplicationBuilder appBuilder;
	return appBuilder.AddEventBus<LoggingEventBus>().Build<PlaygroundApplication>();
}