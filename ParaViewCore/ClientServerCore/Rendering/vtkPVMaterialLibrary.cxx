/*=========================================================================

  Program:   ParaView
  Module:    vtkPVMaterialLibrary.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkPVMaterialLibrary.h"

#include "vtkPVConfig.h"

#include "vtkNew.h"
#include "vtkObjectFactory.h"
#ifdef PARAVIEW_USE_OSPRAY
#include "vtkOSPRayMaterialLibrary.h"
#endif
#include "vtkResourceFileLocator.h"
#include <vtksys/SystemTools.hxx>

#include <cassert>
#include <string>

#if defined(_WIN32) && !defined(__CYGWIN__)
const char ENV_PATH_SEP = ';';
#else
const char ENV_PATH_SEP = ':';
#endif

//-----------------------------------------------------------------------------
vtkStandardNewMacro(vtkPVMaterialLibrary);

//-----------------------------------------------------------------------------
vtkPVMaterialLibrary::vtkPVMaterialLibrary()
{
// initialize the class that will act as library of materials
#ifdef PARAVIEW_USE_OSPRAY
  this->MaterialLibrary = vtkOSPRayMaterialLibrary::New();
#else
  this->MaterialLibrary = nullptr;
#endif

  // now look for any materials that we want preloaded into the application
  this->SearchPaths = nullptr;
  vtksys::String paths;

  // user can define a path via environment variables
  const char* env = vtksys::SystemTools::GetEnv("PV_MATERIALS_PATH");
  if (env)
  {
    paths += env;
  }

  // and we look in some default locations relative to the exe
  auto vtklibs = vtkGetLibraryPathForSymbol(GetVTKVersion);
  //assert(!vtklibs.empty);

  // where materials might be in relation to that
  std::vector<std::string> prefixes = {
#if defined(_WIN32) || defined(__APPLE__)
    "materials"
#else
    "share/paraview-" PARAVIEW_VERSION "/materials"
#endif
  };
  // search
  vtkNew<vtkResourceFileLocator> locator;
  auto resource_dir = locator->Locate(vtklibs, prefixes, "ospray_mats.json");
  if (!resource_dir.empty())
  {
    // append results to search path
    if (paths.size())
    {
      paths += ENV_PATH_SEP;
    }
    paths += resource_dir.c_str();
  }

  // now we are ready
  this->SetSearchPaths(paths.c_str());
}

//-----------------------------------------------------------------------------
vtkPVMaterialLibrary::~vtkPVMaterialLibrary()
{
  this->SetSearchPaths(nullptr);
  if (this->MaterialLibrary)
  {
    this->MaterialLibrary->Delete();
  }
}

//-----------------------------------------------------------------------------
void vtkPVMaterialLibrary::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "SearchPaths: " << (this->SearchPaths ? this->SearchPaths : "(none)") << endl;
}

//-----------------------------------------------------------------------------
void vtkPVMaterialLibrary::ReadRelativeFile(const char* FileName)
{
#ifdef PARAVIEW_USE_OSPRAY
  std::vector<std::string> paths;
  vtksys::SystemTools::Split(this->SearchPaths, paths, ENV_PATH_SEP);
  for (size_t cc = 0; cc < paths.size(); cc++)
  {
    std::vector<std::string> subpaths;
    vtksys::SystemTools::Split(paths[cc].c_str(), subpaths, ';');
    for (size_t scc = 0; scc < subpaths.size(); scc++)
    {
      std::string fullpath = subpaths[scc] + "/" + FileName;
      if (this->ReadFile(fullpath.c_str()))
      {
        return;
      }
    }
  }
#else
  (void)FileName;
#endif
}

//-----------------------------------------------------------------------------
vtkObject* vtkPVMaterialLibrary::GetMaterialLibrary()
{
  return this->MaterialLibrary;
}

//-----------------------------------------------------------------------------
bool vtkPVMaterialLibrary::ReadFile(const char* FileName)
{
#ifdef PARAVIEW_USE_OSPRAY
  return vtkOSPRayMaterialLibrary::SafeDownCast(this->GetMaterialLibrary())->ReadFile(FileName);
#else
  (void)FileName;
  return false;
#endif
}

//-----------------------------------------------------------------------------
bool vtkPVMaterialLibrary::ReadBuffer(const char* FileName)
{
#ifdef PARAVIEW_USE_OSPRAY
  return vtkOSPRayMaterialLibrary::SafeDownCast(this->GetMaterialLibrary())->ReadBuffer(FileName);
#else
  (void)FileName;
  return false;
#endif
}

//-----------------------------------------------------------------------------
const char* vtkPVMaterialLibrary::WriteBuffer()
{
#ifdef PARAVIEW_USE_OSPRAY
  return vtkOSPRayMaterialLibrary::SafeDownCast(this->GetMaterialLibrary())->WriteBuffer();
#else
  return nullptr;
#endif
}
