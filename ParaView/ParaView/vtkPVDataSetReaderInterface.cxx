/*=========================================================================

  Program:   
  Module:    vtkPVDataSetReaderInterface.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2000-2001 Kitware Inc. 469 Clifton Corporate Parkway,
Clifton Park, NY, 12065, USA.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither the name of Kitware nor the names of any contributors may be used
   to endorse or promote products derived from this software without specific 
   prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#include "vtkPVDataSetReaderInterface.h"
#include "vtkPDataSetReader.h"
#include "vtkObjectFactory.h"
#include <ctype.h>
#include "vtkKWLabel.h"
#include "vtkKWPushButton.h"
#include "vtkPVData.h"
#include "vtkPVRenderView.h"
#include "vtkPVWindow.h"
#include "vtkPVRenderView.h"

int vtkPVDataSetReaderInterfaceCommand(ClientData cd, Tcl_Interp *interp,
				       int argc, char *argv[]);

//----------------------------------------------------------------------------
vtkPVDataSetReaderInterface::vtkPVDataSetReaderInterface()
{
  this->CommandFunction = vtkPVDataSetReaderInterfaceCommand;
  this->FileName = NULL;
}

//----------------------------------------------------------------------------
vtkPVDataSetReaderInterface* vtkPVDataSetReaderInterface::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkPVDataSetReaderInterface");
  if(ret)
    {
    return (vtkPVDataSetReaderInterface*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkPVDataSetReaderInterface;
}


//----------------------------------------------------------------------------
vtkPVSource *vtkPVDataSetReaderInterface::CreateCallback()
{
  char *tclName, *extentTclName, *tmp;
  char *outputTclName;
  vtkPDataSetReader *s; 
  vtkDataSet *d;
  vtkPVData *pvd;
  vtkPVSource *pvs;
  vtkPVApplication *pvApp = this->GetPVApplication();
  char* result;
  char *extension;
  int extensionPosition;
  char *endingSlash = NULL;
  int slashPosition;

  // Create a name for the reader.
  if (!this->GetDataFileName())
    {
    pvApp->Script("set newFileName [tk_getOpenFile -filetypes {{{VTK Data Sets} {.vtk}} {{All Files} {.*}}}]");
    result = pvApp->GetMainInterp()->result;
    if (strcmp(result, "") == 0)
      {
      return NULL;
      }
    this->SetDataFileName(result);
    }
  // Get the root name between the last slash and last period.
  slashPosition = 0;
  extensionPosition = strlen(this->DataFileName);
  if ((extension = strrchr(this->DataFileName, '.')))
    {
    extensionPosition = extension - this->DataFileName;
    }
  if ((endingSlash = strrchr(this->DataFileName, '/')))
    {
    slashPosition = endingSlash - this->DataFileName + 1;
    }
  tclName = new char[extensionPosition-slashPosition+1+10];
  strncpy(tclName, this->DataFileName+slashPosition, extensionPosition-slashPosition);
  tclName[extensionPosition-slashPosition] = '\0';

  if (isdigit(tclName[0]))
    {
    // A VTK object name beginning with a digit is invalid.
    tmp = new char[strlen(tclName)+3+10];
    strcpy(tmp, tclName);
    delete [] tclName;
    tclName = new char[strlen(tmp)+1+10];
    sprintf(tclName, "PV%s", tmp);
    delete [] tmp;
    }
  // Append the unique number for the name.
  tmp = new char[strlen(tclName)+1+10];
  strcpy(tmp, tclName);
  delete [] tclName;
  tclName = new char[strlen(tmp)+1 + (this->InstanceCount%10)+1 + 10];
  sprintf(tclName, "%s%d", tmp, this->InstanceCount);
  delete [] tmp;
  
  // Create the vtkSource.
  // Create the object through tcl on all processes.
  s = (vtkPDataSetReader *)(pvApp->MakeTclObject(this->SourceClassName, tclName));
  if (s == NULL)
    {
    vtkErrorMacro("Could not get pointer from object.");
    return NULL;
    }
  
  pvs = vtkPVSource::New();
  pvs->SetPropertiesParent(this->PVWindow->GetMainView()->GetPropertiesParent());
  pvs->SetApplication(pvApp);
  pvs->SetInterface(this);
  pvs->SetVTKSource(s, tclName);
  pvs->SetName(tclName);  
  pvApp->BroadcastScript("%s SetFileName %s",
                         tclName, this->GetDataFileName());
  
  // This is necessary because we need to locally keep a copy of the file name,
  // but we need to set DataFileName to NULL (done late in this method)
  // because otherwise we hang onto that file name and don't get prompted for
  // a filename the next time we create a data set reader.
  this->SetFileName(this->GetDataFileName());
  
  // Add the new Source to the View, and make it current.
  this->PVWindow->GetMainView()->AddComposite(pvs);
  pvs->CreateProperties();
  this->PVWindow->SetCurrentPVSource(pvs);

  // Create the output.
  pvd = vtkPVData::New();
  pvd->SetApplication(pvApp);

  outputTclName = new char[strlen(tclName)+7+10];
  sprintf(outputTclName, "%sOutput", tclName);
  s->UpdateInformation();
  switch (s->GetDataType())
    {
    case VTK_POLY_DATA:
      d = (vtkDataSet *)(pvApp->MakeTclObject("vtkPolyData", outputTclName));
      break;
    case VTK_UNSTRUCTURED_GRID:
      d = (vtkDataSet *)(pvApp->MakeTclObject("vtkUnstructuredGrid", outputTclName));
      break;
    case VTK_STRUCTURED_GRID:
      d = (vtkDataSet *)(pvApp->MakeTclObject("vtkStructuredGrid", outputTclName));
      break;
    case VTK_RECTILINEAR_GRID:
      d = (vtkDataSet *)(pvApp->MakeTclObject("vtkRectilinearGrid", outputTclName));
      break;
    case VTK_STRUCTURED_POINTS:
    case VTK_IMAGE_DATA:
      d = (vtkDataSet *)(pvApp->MakeTclObject("vtkImageData", outputTclName));
      break;
    default:
      vtkErrorMacro("Could not determine output type.");
      pvs->Delete();
      pvd->Delete();
      return NULL;
    }
  pvd->SetVTKData(d, outputTclName);

  // Connect the source and data.
  pvs->SetNthPVOutput(0, pvd);
  // It would be nice to have the vtkPVSource set this up, but for multiple
  // outputs, how do we know the method?
  // Relay the connection to the VTK objects.  
  pvApp->BroadcastScript("%s SetOutput %s", pvs->GetVTKSourceTclName(),
			 pvd->GetVTKDataTclName());   

  extentTclName = new char[strlen(tclName)+11 + (this->InstanceCount%10)+1 + 10];
  sprintf(extentTclName, "%s%dTranslator", tclName, this->InstanceCount);
  pvApp->BroadcastScript("vtkPVExtentTranslator %s", extentTclName);
  pvApp->BroadcastScript("%s SetOriginalSource [%s GetOutput]",
                         extentTclName, pvs->GetVTKSourceTclName());
  pvApp->BroadcastScript("%s SetExtentTranslator %s",
                         pvd->GetVTKDataTclName(), extentTclName);
  // Hold onto name so it can be deleted.
  pvs->SetExtentTranslatorTclName(extentTclName);


  
  pvs->Delete();
  pvd->Delete();

  ++this->InstanceCount;

  pvd->InsertExtractPiecesIfNecessary();

  pvs->AcceptCallback();  

  // so we get prompted for a filename if another data set reader is created
  this->SetDataFileName(NULL);

  delete [] tclName;
  delete [] extentTclName;
  delete [] outputTclName;
  
  return pvs;
}

void vtkPVDataSetReaderInterface::SaveInTclScript(ofstream *file, const char* sourceName)
{
  *file << "\t" << sourceName << " SetFileName " << this->FileName << "\n";
}
