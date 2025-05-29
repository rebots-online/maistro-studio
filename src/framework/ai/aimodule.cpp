/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2023 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "aimodule.h"

#include "global/modularity/ioc.h"
#include "log.h"

using namespace muse;
using namespace muse::modularity;
using namespace muse::ai;

AiModule::AiModule()
{
    // Constructor
}

std::string AiModule::moduleName() const
{
    return "ai_module";
}

void AiModule::registerExports()
{
    // Register exports here if needed
    // Example: ioc()->registerExport<IMyInterface>(moduleName(), std::make_shared<MyImplementation>());
}

void AiModule::registerResources()
{
    // Register QRC resources here if needed
    // Example: Q_INIT_RESOURCE(myresource);
}

#include <QQmlEngine> // Required for qmlRegisterType
#include "AiController.h" // The class we want to register

void AiModule::registerUiTypes()
{
    // Register QML types here
    // Format: qmlRegisterType<CppClassName>("Uri", MajorVersion, MinorVersion, "QmlName");
    qmlRegisterType<Muse::Ai::AiController>("Muse.Ai", 1, 0, "AiController");

    // If AiGenerationPage.qml needs to be found via an import path (it's usually handled by QRC):
    // Example: ioc()->resolve<ui::IUiEngine>(moduleName())->addSourceImportPath(muse_ai_QML_IMPORT);
    // For now, assuming AiGenerationPage.qml is accessed directly via its path in QRC.
}

void AiModule::resolveImports()
{
    // Resolve imports here if needed
    // Example: m_myService = ioc()->resolve<IMyService>(moduleName());
}

void AiModule::onInit(const IApplication::RunMode& mode)
{
    // Module initialization logic
    LOGD("AiModule initialized in mode: {}", static_cast<int>(mode));

    // It might also be a good place to create an instance of AiController
    // if it's meant to be a singleton accessible via C++ through IoC.
    // For now, we're only registering it for QML instantiation.
}

void AiModule::onDeinit()
{
    // Module deinitialization logic
    LOGD("AiModule deinitialized");
}

void AiModule::onDestroy()
{
    // Module destruction logic
    LOGD("AiModule destroyed");
}
