#pragma once

#include <cstdlib>


template <class T>
class Singleton
{
public:
static T& Instance()
{
  if (!Inst)
    Inst = new T;
 return *Inst;
}

static void Destroy ()
  {
    delete Inst;
    Inst = NULL;
  }

protected :
Singleton (void) {}
~Singleton (void) {}

private :
static T* Inst;

Singleton (Singleton&);
Singleton& operator = (const Singleton&);
};

////////////////////////////////////////////////////////////
/// Macro automatisant la création d'un singleton --
/// à placer dans la définition de la classe concernée.
/// La raison pour laquelle il faut redéfinir (et masquer)
/// la fonction Instance() reste obscure, mais si ce n'est pas
/// fait le client obtient des erreurs de link sur Inst
////////////////////////////////////////////////////////////
 #define MAKE_SINGLETON(Class) \
    public : \
        friend class Singleton<Class>; \
        static Class& Instance(); \
    private :

////////////////////////////////////////////////////////////
/// Macro automatisant
////////////////////////////////////////////////////////////
#define SINGLETON_IMPL(Class) \
    template <> Class* Singleton<Class>::Inst = NULL; \
    Class& Class::Instance() \
    { \
        return Singleton<Class>::Instance(); \
    }
