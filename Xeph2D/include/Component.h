#pragma once
#include "Structs.h"
#include <cstdint>

// VVV Place in Body of Register Implementation VVV
#define COMP_SRC_STD 

// VVV Place In Header VVV
#define COMP_HEADER_STD(id) inline static uint32_t typeID = id; uint32_t GetTypeID() override { return typeID; }\
void Register(GameObject* gameObject) override { this->gameObject = gameObject; this->transform = &gameObject->transform; }

namespace Xeph2D
{
	class GameObject;
	class Component
	{
	public:
		Transform* transform = nullptr;
		GameObject* gameObject = nullptr;

		struct Properties
		{
			bool hasPhysics = false;
		};
		Properties properties = {};

	private:
		bool _enabled;

	public:
		virtual void Register(GameObject* gameObject) = 0;

		virtual void EditorInit() {};
		virtual void EditorShutdown() {};

		virtual void Awake() {}
		virtual void Start() {}
		virtual void OnEnable() {}
		virtual void EarlyUpdate() {}
		virtual void Update() {}
		virtual void LateUpdate() {}
		virtual void DebugDraw() {};
		virtual void OnDisable() {}
		virtual void OnDestroy() {}

		void SetEnabled(const bool enabled);
		bool GetEnabled() const { return _enabled; }
		bool IsActiveAndEnabled() const { return true; } // TODO

		virtual uint32_t GetTypeID() = 0;
	};
}