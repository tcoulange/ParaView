/*=========================================================================

   Program: ParaView
   Module:    pqXDMFPanel.h

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.1. 

   See License_v1.1.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
cxxPROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#ifndef _pqXDMFPanel_h
#define _pqXDMFPanel_h

#include "pqNamedObjectPanel.h"
#include "pqComponentsExport.h"

class QListWidgetItem;
class pqXDMFPanelArrayRecord;

// a panel for XDMF readers
// allows user to choose a domain name and select from the available groups
class PQCOMPONENTS_EXPORT pqXDMFPanel :
  public pqNamedObjectPanel
{
  Q_OBJECT

public:
  /// constructor
  pqXDMFPanel(pqProxy* proxy, QWidget* p = NULL);
  /// destructor
  ~pqXDMFPanel();

signals:

public slots:

protected:
  /// populate widgets with properties from the server manager
  virtual void linkServerManagerProperties();

  // fill the domain selection part of the GUI
  void PopulateDomainWidget();

  // fill the grid selection part of the GUI
  void PopulateGridWidget();

  // ask the server what the selection state of the arrays is
  void ResetArrays();

  // fill the array selection part of the GUI
  void PopulateArrayWidget();

  // fill the parameters part of the GUI
  void PopulateParameterWidget();

  // overridden to make domain and grid choices const after 1st selection
  virtual void accept();

  class pqUI;
  pqUI* UI;

protected slots:
  void SetSelectedDomain(QString newDomain);
  void SetSelectedGrids();
  void RecordLastSelectedGrid(QListWidgetItem *);
  void SetCellValue(int r);

private:
  QList<pqXDMFPanelArrayRecord> ArrayList;
  QListWidgetItem *LastGridDeselected;
  bool NeedsResetGrid;
  bool FirstAcceptHappened;
};

#endif

