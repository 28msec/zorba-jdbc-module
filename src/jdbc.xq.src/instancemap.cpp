/*
 * Copyright 2006-2012 The FLWOR Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "instancemap.h"
#include "jdbc.h"
#include "jni.h"


namespace zorba
{
namespace jdbc
{

InstanceMap::InstanceMap()
{   
  InstanceMap::instanceMap = new InstanceMap_t();
}

bool
InstanceMap::storeInstance(const String& aKeyName, jobject aInstance)
{
  std::pair<InstanceMap_t::iterator, bool> ret;
  ret = instanceMap->insert(std::pair<String, jobject>(aKeyName, aInstance));
  return ret.second;
}

jobject
InstanceMap::getInstance(const String& aKeyName)
{
  InstanceMap::InstanceMap_t::iterator lIter = instanceMap->find(aKeyName);
  
  if (lIter == instanceMap->end())
    return NULL;
  
  jobject lInstance = lIter->second;

  return lInstance;
}

bool
InstanceMap::deleteInstance(const String& aKeyName)
{
  InstanceMap::InstanceMap_t::iterator lIter = instanceMap->find(aKeyName);

  if (lIter == instanceMap->end())
    return false;

  //lIter->second;
  //Example: do anything you need to finalize object

  instanceMap->erase(lIter);

  return true;
}

void
InstanceMap::destroy() throw()
{
  if (instanceMap)
  {
    for (InstanceMap_t::const_iterator lIter = instanceMap->begin();
         lIter != instanceMap->end(); ++lIter)
    {
      //lIter->second;
      //Example: do anything you need to finalize object
    }
    instanceMap->clear();
    delete instanceMap;
  }
  delete this;
};

}}; // namespace zorba, jdbc
