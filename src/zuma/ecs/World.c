
#include "World.h"

#include "global/HQC.h"

typedef struct ComponentNode {
  int id;
  void *component;
  Entity *owner;

  struct ComponentNode *prevNode;
} ComponentNode;

typedef struct SystemNode {
  System system;
  struct SystemNode *prevNode;
} SystemNode;

typedef struct {
  ComponentNode *componentListHead;
  SystemNode *systemListHead;
} World_;

struct AbstractListNode {
  void *data;
  struct AbstractListNode *prev;
};

void linkedListDestroy(struct AbstractListNode *head) {
  for (struct AbstractListNode *node = head; node != NULL;) {
    node = node->prev;
    HQC_Memory_Free(node->data);
    HQC_Memory_Free(node);
  }
}

World *World_Create() {
  World_ *world = (World_ *)HQC_Memory_Allocate(sizeof(World_));

  world->componentListHead = NULL;
  world->systemListHead = NULL;

  return (World *)world;
}

void World_Destroy(World *world) {
  World_ *w = (World_ *)world;

  linkedListDestroy((struct AbstractListNode *)w->systemListHead);
  linkedListDestroy((struct AbstractListNode *)w->componentListHead);

  HQC_Memory_Free(world);
}

void World_AddSystem(World *world, System system) {
  World_ *w = (World_ *)world;

  if (w->systemListHead == NULL) {
    w->systemListHead = HQC_Memory_Allocate(sizeof(*w->systemListHead));

    w->systemListHead->system = system;
    w->systemListHead->prevNode = NULL;

    for (struct ComponentNode *componentNode = w->componentListHead;
         componentNode != NULL; componentNode = componentNode->prevNode) {
      if (w->systemListHead->system.onCreate == NULL)
        continue;

      w->systemListHead->system.onCreate(
          componentNode->id, componentNode->component, componentNode->owner);
    }

    return;
  }

  SystemNode *newNode = HQC_Memory_Allocate(sizeof(*newNode));

  newNode->system = system;
  newNode->prevNode = w->systemListHead;

  for (struct ComponentNode *componentNode = w->componentListHead;
       componentNode != NULL; componentNode = componentNode->prevNode) {
    if (newNode->system.onCreate == NULL)
      continue;

    newNode->system.onCreate(componentNode->id, componentNode->component,
                             componentNode->owner);
  }

  w->systemListHead = newNode;
}

void World_RunSystems(World *world) {
  World_ *w = (World_ *)world;

  for (SystemNode *systemNode = w->systemListHead; systemNode->prevNode != NULL;
       systemNode = systemNode->prevNode) {
    for (ComponentNode *componentNode = w->componentListHead;
         componentNode != NULL; componentNode = componentNode->prevNode) {
      if (systemNode->system.onUpdate == NULL)
        continue;

      systemNode->system.onUpdate(componentNode->id, componentNode->component,
                                  componentNode->owner);
    }
  }
}

void World_DrawSystems(World *world) {
  World_ *w = (World_ *)world;

  for (SystemNode *systemNode = w->systemListHead; systemNode != NULL;
       systemNode = systemNode->prevNode) {
    for (ComponentNode *componentNode = w->componentListHead;
         componentNode != NULL; componentNode = componentNode->prevNode) {
      if (systemNode->system.onDraw == NULL)
        continue;

      systemNode->system.onDraw(componentNode->id, componentNode->component,
                                componentNode->owner);
    }
  }
}

//////////

Entity *Entity_Create(World *world) {
  Entity *ent = HQC_Memory_Allocate(sizeof(*ent));

  ent->world = world;

  return ent;
}

void Entity_Destroy(Entity *entity) { HQC_Memory_Free(entity); }

void *Entity_AddComponent(Entity *entity, int tagId, void *component,
                          long componentSize) {
  World_ *world = entity->world;

  if (world->componentListHead == NULL) {
    world->componentListHead =
        HQC_Memory_Allocate(sizeof(*world->componentListHead));

    world->componentListHead->component = HQC_Memory_Allocate(componentSize);

    HQC_Memory_Copy(world->componentListHead->component, component,
                    componentSize);

    world->componentListHead->id = tagId;
    world->componentListHead->owner = entity;
    world->componentListHead->prevNode = NULL;

    return world->componentListHead->component;
  }

  ComponentNode *newNode = HQC_Memory_Allocate(sizeof(*newNode));

  newNode->component = HQC_Memory_Allocate(componentSize);

  HQC_Memory_Copy(newNode->component, component, componentSize);

  newNode->id = tagId;
  newNode->owner = entity;
  newNode->prevNode = world->componentListHead;

  world->componentListHead = newNode;

  return newNode->component;
}

void *Entity_QueryComponent(Entity *entity, int tagId) {
  World_ *world = entity->world;

  for (ComponentNode *componentNode = world->componentListHead;
       componentNode != NULL; componentNode = componentNode->prevNode) {

    if (componentNode->id == tagId && componentNode->owner == entity) {
      return componentNode->component;
    }
  }

  return NULL;
}
