/*=========================================================================

   Program: ParaView
   Module:    $RCSfile$

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
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
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#include "pqProxyWidgetDialog.h"
#include "ui_pqProxyWidgetDialog.h"

#include "pqCoreUtilities.h"
#include "pqProxyWidget.h"

#include "vtkCommand.h"
#include "vtkSmartPointer.h"
#include "vtkSMProperty.h"
#include "vtkSMPropertyIterator.h"
#include "vtkSMProxy.h"
#include "vtkSMSettings.h"
#include "vtkSMVectorProperty.h"

#include <QScrollArea>
#include <QStyle>

class pqProxyWidgetDialog::pqInternals
{
public:
  Ui::ProxyWidgetDialog Ui;

  pqInternals(vtkSMProxy* proxy, pqProxyWidgetDialog* self, 
    const QStringList& properties = QStringList()) :
    Proxy(proxy)
    {
    Q_ASSERT(proxy != NULL);

    Ui::ProxyWidgetDialog& ui = this->Ui;
    ui.setupUi(self);

    // There should be no changes initially, so disable the Apply button
    ui.ApplyButton->setEnabled(false);

    QWidget *container = new QWidget(self);
    container->setObjectName("Container");
    QVBoxLayout* vbox = new QVBoxLayout(container);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);

    // Set up the widget for the proxy
    pqProxyWidget *widget = properties.size() > 0?
      new pqProxyWidget(proxy, properties, container):
      new pqProxyWidget(proxy, container);
    widget->setObjectName("ProxyWidget");
    widget->filterWidgets(true);
    vbox->addWidget(widget);

    // Set some icons for the buttons
    QStyle* applicationStyle = QApplication::style();
    ui.RestoreDefaultsButton->
      setIcon(applicationStyle->standardIcon(QStyle::SP_BrowserReload));
    ui.SaveButton->
      setIcon(applicationStyle->standardIcon(QStyle::SP_DialogSaveButton));
    ui.ApplyButton->
      setIcon(applicationStyle->standardIcon(QStyle::SP_DialogApplyButton));
    ui.CancelButton->
      setIcon(applicationStyle->standardIcon(QStyle::SP_DialogCancelButton));
    ui.OKButton->
      setIcon(applicationStyle->standardIcon(QStyle::SP_DialogOkButton));

    QObject::connect(self, SIGNAL(accepted()), widget, SLOT(apply()));
    QObject::connect(self, SIGNAL(accepted()), self, SLOT(onAccepted()));
    QObject::connect(widget, SIGNAL(changeAvailable()),
       self, SLOT(onChangeAvailable()));

    // When restoring defaults, first restore the defaults in the
    // server manager, then reset the values from the server manager
    // after the defaults have been restored.
    QObject::connect(ui.RestoreDefaultsButton, SIGNAL(clicked()),
      self, SLOT(onRestoreDefaults()));
    QObject::connect(ui.RestoreDefaultsButton, SIGNAL(clicked()),
       widget, SLOT(reset()));

    QObject::connect(ui.SaveButton, SIGNAL(clicked()),
      self, SLOT(onSaveAsDefaults()));

    QObject::connect(ui.ApplyButton, SIGNAL(clicked()), self, SLOT(accept()));

    QObject::connect(ui.CancelButton, SIGNAL(clicked()), widget, SLOT(reset()));
    QObject::connect(ui.CancelButton, SIGNAL(clicked()), self, SLOT(reject()));

    QObject::connect(ui.OKButton, SIGNAL(clicked()), self, SLOT(accept()));
    QObject::connect(ui.OKButton, SIGNAL(clicked()), self, SLOT(close()));

    QSpacerItem* spacer = new QSpacerItem(0, 6, QSizePolicy::Fixed,
      QSizePolicy::MinimumExpanding);
    vbox->addItem(spacer);

    /// Setup the scroll area. Its minimum size is set to fully contain the
    /// proxy widget so we're sure it'll be completly displayed
    ui.scrollArea->setWidget(container);
    QSize oldMinSize = ui.scrollArea->minimumSize();
    ui.scrollArea->setMinimumSize(container->size());

    /// Get the dialog size. It's the layout minSize since the widget
    /// can't be any smaller right now.
    QSize dialogSize = self->layout()->minimumSize();

    /// Reset the scroll area so it can actually be used.
    ui.scrollArea->setMinimumSize(oldMinSize);

    // Finaly set the maximum and current dialog size
    self->setMaximumSize(pqCoreUtilities::mainWidget()->size());
    self->resize(dialogSize);
    }

  vtkSMProxy* Proxy;
};

//-----------------------------------------------------------------------------
pqProxyWidgetDialog::pqProxyWidgetDialog(vtkSMProxy* proxy, QWidget* parentObject)
  : Superclass(parentObject),
  Internals(new pqProxyWidgetDialog::pqInternals(proxy, this))
{
}

//-----------------------------------------------------------------------------
pqProxyWidgetDialog::pqProxyWidgetDialog(vtkSMProxy* proxy,
  const QStringList& properties, QWidget* parentObject)
  : Superclass(parentObject),
  Internals(new pqProxyWidgetDialog::pqInternals(proxy, this, properties))
{
}

//-----------------------------------------------------------------------------
pqProxyWidgetDialog::~pqProxyWidgetDialog()
{
  delete this->Internals;
  this->Internals = NULL;
}

//-----------------------------------------------------------------------------
void pqProxyWidgetDialog::accept()
{
  emit this->accepted();
}

//-----------------------------------------------------------------------------
void pqProxyWidgetDialog::onChangeAvailable()
{
  Ui::ProxyWidgetDialog &ui = this->Internals->Ui;
  ui.ApplyButton->setEnabled(true);
}

//-----------------------------------------------------------------------------
void pqProxyWidgetDialog::onAccepted()
{
  Ui::ProxyWidgetDialog &ui = this->Internals->Ui;
  ui.ApplyButton->setEnabled(false);
}

//-----------------------------------------------------------------------------
void pqProxyWidgetDialog::onRestoreDefaults()
{
  bool anyReset = false;
  if (this->Internals->Proxy)
    {
    vtkSmartPointer<vtkSMPropertyIterator> iter;
    iter.TakeReference(this->Internals->Proxy->NewPropertyIterator());
    for (iter->Begin(); !iter->IsAtEnd(); iter->Next())
      {
      vtkSMProperty * property = iter->GetProperty();
      // Restore only basic type properties.
      if (vtkSMVectorProperty::SafeDownCast(property) &&
          !property->GetNoCustomDefault() &&
          !property->GetInformationOnly())
        {
        if (!property->IsValueDefault())
          {
          anyReset = true;
          }
        property->ResetToXMLDefaults();
        }
      }
    }

  // The code above bypasses the changeAvailable() signal from the
  // pqProxyWidget, so we check here whether we should act as if
  // changes are available only if any of the properties have been
  // reset.
  if (anyReset)
    {
    this->onChangeAvailable();
    }
}

//-----------------------------------------------------------------------------
void pqProxyWidgetDialog::onSaveAsDefaults()
{
  vtkSMSettings* settings = vtkSMSettings::GetInstance();
  settings->SetProxySettings(this->Internals->Proxy);
}
