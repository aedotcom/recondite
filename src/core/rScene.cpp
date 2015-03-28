#include "rScene.hpp"

rScene::rScene(rEngine* engine){
	m_isLoading = false;

	m_engine = engine;
}

rScene::~rScene(){
	Clear();
}

void rScene::Update(){
	rActorArray actorsToDelete;
	rActorMap::iterator end = m_actors.end();
	int remove;

	for (rActorMap::iterator it = m_actors.begin(); it != end; ++it){
		remove = it->second->Update();

		if (remove)
			actorsToDelete.push_back(it->second);
	}

	for (size_t i = 0; i < actorsToDelete.size(); i++)
		DeleteActor(actorsToDelete[i]->Id());
}

void rScene::Draw(){
	//todo: view frustrum culling
	rActorMap::iterator end = m_actors.end();

	for (rActorMap::iterator it = m_actors.begin(); it != end; ++it)
		it->second->Draw();
}

void rScene::AddActor(rActor3* actor){
	rString name = actor->Id();
	
	if (m_actors.count(name)){
		DeleteActor(name);
	}

	m_actors[name] = actor;

	if (!m_isLoading){
		rActor3Event event(actor);
		Trigger(rEVT_SCENE_ACTOR_ADDED, event);
	}

}

rActor3* rScene::GetActor(const rString& name) const{
	rActor3* actor = NULL;

	rActorMap::const_iterator result = m_actors.find(name);

	if (result != m_actors.end())
		actor = result->second;

	return actor;
}

void rScene::DeleteActor(const rString& name){
	rActorMap::iterator result = m_actors.find(name);

	if (result != m_actors.end()){
		rActor3Event event(result->second);
		Trigger(rEVT_SCENE_ACTOR_REMOVED, event);

		delete result->second;
		m_actors.erase(result);
	}
}

void rScene::Clear(){
	rActorMap::iterator end = m_actors.end();

	for (rActorMap::iterator it = m_actors.begin(); it != end; ++it)
		delete it->second;

	m_actors.clear();
}

size_t rScene::NumActors () const{
	return m_actors.size();
}

rString rScene::GetDefaultActorId(const rString& prefix){
	rString defaultActorId;
	int count = 0;

	do {
		rOStringStream str(prefix);
		str << count;
		defaultActorId = str.Str();
		count += 1;
	}while(m_actors.count(defaultActorId) == 1);

	return defaultActorId;
}

rActor3* rScene::RayPick(const rRay3& ray){
	rActor3* selectedActor = NULL;
	float selectedActorDistance = FLT_MAX;
	rVector3 selectionPoint;
	

	rActorMap::iterator end = m_actors.end();
	for (rActorMap::iterator it = m_actors.begin(); it != end; ++it){
		rActor3* currentActor = it->second;
		riBoundingVolume* boundingVolume = currentActor->BoundingVolume();

		if (boundingVolume && boundingVolume->IntersectsRay(ray, &selectionPoint)){
			float currentActorDistance = ray.origin.Distance(selectionPoint);
			if (currentActorDistance < selectedActorDistance){
				selectedActor = currentActor;
				selectedActorDistance = currentActorDistance;
			}
		}
	}

	return selectedActor;
}

rActor3* rScene::ViewportPick(const rString& viewportName, int x, int y){
	/*
	rActor3* selectedActor = NULL;

	rViewport* viewport = m_component->GetViewport("main");
	rSize size = viewport->Size();

	unsigned int renderBufferId = m_graphicsDevice->CreateRenderbuffer(size.x, size.y);

	//render for selection
	m_graphicsDevice->Clear();
	int pixelColor = m_graphicsDevice->ReadRenderbufferPixel(1,1);

	m_graphicsDevice->DeleteRenderbuffer(renderBufferId);

	return selectedActor;
	*/

	return nullptr;
}

bool rScene::Save(riSerializationTarget* target){
	riSerializationTarget* actorTarget = target->SubObject("actors");

	for (auto& actor : m_actors)
		actor.second->Save(actorTarget);

	return true;
}

bool rScene::Load(riSerializationTarget* target){
	Clear();

	m_isLoading = true;
	rEvent event;
	Trigger(rEVT_SCENE_LOAD_BEGIN, event);

	riSerializationTarget* actorsTarget = target->SubObject("actors");
	riSerializationTarget* actorTarget = actorsTarget->SubObject("actor");
	
	if (actorTarget){
		rString className, id;

		do{
			actorTarget->String("class", className);
			actorTarget->String("id", id);

			rActor3* actor = m_engine->actors->GetActorClass(className, m_engine, id);
			if (actor){
				actor->Load(actorTarget);
				AddActor(actor);
			}
			else{
				rLog::Warning("Unable to load actor with class: " + className);
			}
		} while (actorTarget->Next());
	}

	m_isLoading = false;
	Trigger(rEVT_SCENE_LOAD_END, event);

	return true;
}

bool rScene::IsLoading() const{
	return m_isLoading;
}

void rScene::GetActors(rArrayString& names) const{
	for (auto& entry : m_actors)
		names.push_back(entry.first);
}