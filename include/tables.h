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

#include <zorba/empty_sequence.h>
#include <zorba/external_module.h>
#include <zorba/item_factory.h>
#include <zorba/zorba.h>

#include "JavaVMSingleton.h"


namespace zorba
{
namespace jdbc
{

class TablesFunction : public ContextualExternalFunction
{
  private:
    const ExternalModule* theModule;
    ItemFactory* theFactory;
    XmlDataManager* theDataManager;

  public:
    TablesFunction(const ExternalModule* aModule);

    virtual ~TablesFunction();

  public:
    virtual String getURI() const;

    virtual String getLocalName() const;

    virtual ItemSequence_t 
      evaluate(const ExternalFunction::Arguments_t& args,
               const zorba::StaticContext*,
               const zorba::DynamicContext*) const;
};

class TablesNondeterministicFunction : public TablesFunction
{
  public:
    TablesNondeterministicFunction(const ExternalModule* aModule):
      TablesFunction(aModule)
    {};

    virtual ~TablesNondeterministicFunction() {};

  public:
    virtual String getLocalName() const
    {
      return "tables-nondeterministic";
    }
};

}} // namespace zorba, jdbc
