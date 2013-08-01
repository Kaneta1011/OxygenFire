#ifndef __txSingleton_H__
#define __txSingleton_H__

#if 0

namespace StandardLib{

//----------------------------------------------------------------------
//	Singleton
//----------------------------------------------------------------------
template <class _Ty>
class Singleton
{
public :
  typedef _Ty InstanceType;
public :
	//	Create
  static void Create(){ if(_instance == NULL){_instance = new InstanceType(); } }
  //	Delete
	static void Delete(){ if(_instance != NULL){ delete _instance; _instance = NULL;} }
	//	get
  static InstanceType* getInstance(){ return _instance; }
  //	is
  static bool isCreate()  { return _instance != NULL; };
  static bool isDestroy() { return _instance == NULL; };
private :
	//	Data
  static InstanceType* _instance;
  //	BasicSet
  Singleton() {}
  virtual ~Singleton() {}
  Singleton(const Singleton&);
  Singleton& operator=(const Singleton&);
};
 
template <class _Ty>
typename Singleton<_Ty>::InstanceType* Singleton<_Ty>::_instance = NULL;

}//Standard

#endif



#endif