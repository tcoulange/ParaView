/*=========================================================================

  Module:    vtkKWTextProperty.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkKWTextProperty.h"

#include "vtkActor2D.h"
#include "vtkKWApplication.h"
#include "vtkKWChangeColorButton.h"
#include "vtkKWCheckButton.h"
#include "vtkKWCheckButtonSet.h"
#include "vtkKWLabel.h"
#include "vtkKWLabeledCheckButtonSet.h"
#include "vtkKWLabeledOptionMenu.h"
#include "vtkKWLabeledPushButtonSet.h"
#include "vtkKWOptionMenu.h"
#include "vtkKWPushButton.h"
#include "vtkKWPushButtonSet.h"
#include "vtkKWScale.h"
#include "vtkKWTkUtilities.h"
#include "vtkObjectFactory.h"
#include "vtkProperty2D.h"
#include "vtkTextProperty.h"

#define VTK_KW_TEXT_PROP_ARIAL   "Arial"
#define VTK_KW_TEXT_PROP_COURIER "Courier"
#define VTK_KW_TEXT_PROP_TIMES   "Times"

#define VTK_KW_TEXT_PROP_BOLD_ID   0
#define VTK_KW_TEXT_PROP_ITALIC_ID 1
#define VTK_KW_TEXT_PROP_SHADOW_ID 2

#define VTK_KW_TEXT_PROP_COPY_ID 0

/* 
 * This part was generated by ImageConvert from image:
 *    bold.png (zlib, base64)
 */
#define image_bold_width         12
#define image_bold_height        16
#define image_bold_pixel_size    4
#define image_bold_buffer_length 152

static unsigned char image_bold[] = 
  "eNr7//8/w38aYiBQAuI0IH4HxP+huByIZ0LFzkD56GA3knoYMEYzg5B6ZPF3NFCP7J40PO"
  "ph/gWx7wKxCxH+VULS846A+cgAOZxdSPAvepjiUn8Gh58Jhc8ZJH+hp4dVQNyBlh4EQYr/"
  "0zh9AgBC7tWi";

/* 
 * This part was generated by ImageConvert from image:
 *    italic.png (zlib, base64)
 */
#define image_italic_width         12
#define image_italic_height        16
#define image_italic_pixel_size    4
#define image_italic_buffer_length 140

static unsigned char image_italic[] = 
  "eNr7//8/w38aYyzABYjvAjFIsgOIlQioB4HdUD1gQIT6d0A8k0j1LlC3hBKpvgOqXpBI9W"
  "eg7mcgQr0S1OxyItWnQdUrEal+FXI4EqH+HdS/xKgPRQ9HHOoFoWrOIKcBXOppiQHpUhBU";

/* 
 * This part was generated by ImageConvert from image:
 *    shadow.png (zlib, base64)
 */
#define image_shadow_width         12
#define image_shadow_height        16
#define image_shadow_pixel_size    4
#define image_shadow_buffer_length 384

static unsigned char image_shadow[] = 
  "eNq10s8rRFEYxvFhRmZIzSATXSnNlR/JwsiMsiALyo+sxFqUmhVFdjaSBWv/go2VlY38Bf"
  "4DS6uJsrC8vk89i9PZO/Xpdmeec977vvdmWZbL/pnXOR58fcQN1nLBCvJHznShH8N4wz6q"
  "6EM+yL/gFgmaWMIFTl1jEqUgr7O//P8u5jCCA2xiQTWC/Dy+9Yh4Rl31Ucasz++N+t3Ah/"
  "f84EQZ7xlAd5RXT8t4wq/3XWMIRXQG+XHdYxAruMQn2tjBKApBXjPv8DxVfwZ3rnGFLT1T"
  "NE/NseLaBc/q1XNtIY3m+Y577OHYZ0z4Xr1PB3mdO4ZDnHlWZfe17R6SaD76Btb9vhruYR"
  "GrmIrer1bJ30OKmq+pf+vRPJT9A5tnxYs=";

/* 
 * This part was generated by ImageConvert from image:
 *    copy.png (zlib, base64)
 */
#define image_copy_width         15
#define image_copy_height        16
#define image_copy_pixel_size    4
#define image_copy_buffer_length 124

static unsigned char image_copy[] = 
  "eNqlkwkKACAIBHu6P9+CLjNlpYSlCKZJJAAFj2kFL1nWVpaPWLVSVrnGXtwwb+fFPdO89e"
  "5IeN/kmffsxfaQ997vz3nN7Bfrh3vj2XEvY1+9LPj4Rz+pVM55tQ==";

// ----------------------------------------------------------------------------
vtkStandardNewMacro(vtkKWTextProperty);
vtkCxxRevisionMacro(vtkKWTextProperty, "1.32");

int vtkKWTextPropertyCommand(ClientData cd, Tcl_Interp *interp,
                      int argc, char *argv[]);

// ----------------------------------------------------------------------------
vtkKWTextProperty::vtkKWTextProperty()
{
  this->TextProperty = NULL;
  this->Actor2D = NULL;

  this->LongFormat = 0;
  this->LabelOnTop = 1;

  this->ShowLabel = 0;
  this->Label = vtkKWLabel::New();

  this->ShowColor = 1;
  this->ChangeColorButton = vtkKWChangeColorButton::New();

  this->ShowFontFamily = 1;
  this->FontFamilyOptionMenu = vtkKWLabeledOptionMenu::New();

  this->ShowStyles = 1;
  this->StylesCheckButtonSet = vtkKWLabeledCheckButtonSet::New();

  this->ShowOpacity = 1;
  this->OpacityScale = vtkKWScale::New();

  this->ChangedCommand = NULL;
  this->ColorChangedCommand = NULL;

  this->ShowCopy = 0;
  this->PushButtonSet = vtkKWLabeledPushButtonSet::New();
}

// ----------------------------------------------------------------------------
vtkKWTextProperty::~vtkKWTextProperty()
{
  this->SetTextProperty(NULL);
  this->SetActor2D(NULL);

  if (this->ChangedCommand)
    {
    delete [] this->ChangedCommand;
    this->ChangedCommand = NULL;
    }

  if (this->ColorChangedCommand)
    {
    delete [] this->ColorChangedCommand;
    this->ColorChangedCommand = NULL;
    }

  if (this->Label)
    {
    this->Label->Delete();
    this->Label = NULL;
    }

  if (this->ChangeColorButton)
    {
    this->ChangeColorButton->Delete();
    this->ChangeColorButton = NULL;
    }

  if (this->FontFamilyOptionMenu)
    {
    this->FontFamilyOptionMenu->Delete();
    this->FontFamilyOptionMenu = NULL;
    }

  if (this->StylesCheckButtonSet)
    {
    this->StylesCheckButtonSet->Delete();
    this->StylesCheckButtonSet = NULL;
    }

  if (this->OpacityScale)
    {
    this->OpacityScale->Delete();
    this->OpacityScale = NULL;
    }

  if (this->PushButtonSet)
    {
    this->PushButtonSet->Delete();
    this->PushButtonSet = NULL;
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::Create(vtkKWApplication *app)
{
  // Set the application

  if (this->IsCreated())
    {
    vtkErrorMacro("vtkKWTextProperty already created");
    return;
    }
  this->SetApplication(app);

  // Create container

  this->Script("frame %s -borderwidth 0 -relief flat", this->GetWidgetName());

  // Label

  this->Label->SetParent(this);
  this->Label->Create(app, "");

  // Color

  this->ChangeColorButton->SetParent(this);
  this->ChangeColorButton->LabelOutsideButtonOn();
  this->ChangeColorButton->Create(app, "");
  this->ChangeColorButton->SetCommand(this, "ChangeColorButtonCallback");
  this->ChangeColorButton->SetBalloonHelpString("Select the text color.");
  this->ChangeColorButton->SetDialogText("Text Color");

  // Font Family

  this->FontFamilyOptionMenu->SetParent(this);
  this->FontFamilyOptionMenu->Create(app);
  this->Script("%s config -width 7 -padx 0 -pady 2", 
               this->FontFamilyOptionMenu->GetOptionMenu()->GetWidgetName());
  this->FontFamilyOptionMenu->SetBalloonHelpString("Select the font.");

  this->FontFamilyOptionMenu->GetOptionMenu()->AddEntryWithCommand(
    VTK_KW_TEXT_PROP_ARIAL, this, "FontFamilyCallback");

  this->FontFamilyOptionMenu->GetOptionMenu()->AddEntryWithCommand(
    VTK_KW_TEXT_PROP_COURIER, this, "FontFamilyCallback");

  this->FontFamilyOptionMenu->GetOptionMenu()->AddEntryWithCommand(
    VTK_KW_TEXT_PROP_TIMES, this, "FontFamilyCallback");

  // Styles (bold, italic, shadow)

  this->StylesCheckButtonSet->SetParent(this);
  this->StylesCheckButtonSet->PackHorizontallyOn();
  this->StylesCheckButtonSet->Create(app);
  this->StylesCheckButtonSet->SetBalloonHelpString("Select the font style.");

  vtkKWCheckButtonSet *cbs = this->StylesCheckButtonSet->GetCheckButtonSet();
  cbs->PackHorizontallyOn();

  vtkKWCheckButton *cb;
  const char *styles_options = "-indicator 0 -padx 0 -pady 0";

  cbs->AddButton(VTK_KW_TEXT_PROP_BOLD_ID, 
                 0,
                 this, "BoldCallback",
                 "Select bold style.");
  cb = cbs->GetButton(VTK_KW_TEXT_PROP_BOLD_ID);
  this->Script("%s configure %s", cb->GetWidgetName(), styles_options);

  cb->SetImageOption(image_bold, 
                     image_bold_width, 
                     image_bold_height, 
                     image_bold_pixel_size,
                     image_bold_buffer_length);

  cbs->AddButton(VTK_KW_TEXT_PROP_ITALIC_ID, 
                 0, 
                 this, "ItalicCallback",
                 "Select italic style.");
  cb = cbs->GetButton(VTK_KW_TEXT_PROP_ITALIC_ID);
  this->Script("%s configure %s", cb->GetWidgetName(), styles_options);

  cb->SetImageOption(image_italic, 
                     image_italic_width, 
                     image_italic_height, 
                     image_italic_pixel_size,
                     image_italic_buffer_length);

  cbs->AddButton(VTK_KW_TEXT_PROP_SHADOW_ID, 
                 0, 
                 this, "ShadowCallback",
                 "Select shadow style.");
  cb = cbs->GetButton(VTK_KW_TEXT_PROP_SHADOW_ID);
  this->Script("%s configure %s", cb->GetWidgetName(), styles_options);

  cb->SetImageOption(image_shadow, 
                     image_shadow_width, 
                     image_shadow_height, 
                     image_shadow_pixel_size,
                     image_shadow_buffer_length);

  // Opacity

  this->OpacityScale->SetParent(this);
  this->OpacityScale->PopupScaleOn();
  this->OpacityScale->SetResolution(0.01);
  this->OpacityScale->SetRange(0.0, 1.0);
  this->OpacityScale->Create(app, "");
  this->OpacityScale->DisplayEntry();
  this->OpacityScale->DisplayLabel("");
  this->OpacityScale->SetEntryWidth(4);
  this->OpacityScale->DisplayEntryAndLabelOnTopOff();
  this->OpacityScale->SetCommand(this, "OpacityCallback");
  this->OpacityScale->SetEndCommand(this, "OpacityEndCallback");
  this->OpacityScale->SetEntryCommand(this, "OpacityEndCallback");
  this->OpacityScale->SetBalloonHelpString("Select the text opacity.");

  // Buttons

  this->PushButtonSet->SetParent(this);
  this->PushButtonSet->PackHorizontallyOn();
  this->PushButtonSet->Create(app);

  vtkKWPushButtonSet *pbs = this->PushButtonSet->GetPushButtonSet();
  pbs->PackHorizontallyOn();

  vtkKWPushButton *pb;

  // Buttons : copy button

  pbs->AddButton(VTK_KW_TEXT_PROP_COPY_ID);
  pb = pbs->GetButton(VTK_KW_TEXT_PROP_COPY_ID);

  pb->SetImageOption(image_copy, 
                     image_copy_width, 
                     image_copy_height, 
                     image_copy_pixel_size,
                     image_copy_buffer_length);

  // Pack

  this->Pack();

  // Update

  this->UpdateInterface();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::Pack()
{
  if (!this->IsCreated())
    {
    return;
    }

  ostrstream tk_cmd;

  this->ChangeColorButton->UnpackSiblings();

  if (this->LongFormat)
    {
    this->ChangeColorButton->SetLabel("Color:");
    this->ChangeColorButton->ShowLabelOn();

    this->FontFamilyOptionMenu->GetOptionMenu()->IndicatorOn();
    this->FontFamilyOptionMenu->SetLabel("Font:");
    this->FontFamilyOptionMenu->ShowLabelOn();

    this->StylesCheckButtonSet->SetLabel("Style:");
    this->StylesCheckButtonSet->ShowLabelOn();

    this->OpacityScale->DisplayLabel("Opacity:");

    this->PushButtonSet->SetLabel("Functions:");
    this->PushButtonSet->ShowLabelOn();

    int row = 0, col = 0;

    tk_cmd << "grid " 
           << this->Label->GetWidgetName() 
           << " -sticky nsw -column " << col << " -row " << row << endl;

    if (this->LabelOnTop)
      {
      row++;
      }
    else
      {
      col = 1;
      }

    const char *options = " -sticky nsw -pady 1 -padx 10";
    
    tk_cmd << "grid " << this->ChangeColorButton->GetWidgetName() << options 
           << " -column " << col << " -row " << row++ << endl;
    tk_cmd << "grid " << this->FontFamilyOptionMenu->GetWidgetName() << options 
           << " -column " << col << " -row " << row++ << endl;
    tk_cmd << "grid " << this->StylesCheckButtonSet->GetWidgetName() << options 
           << " -column " << col << " -row " << row++ << endl;
    tk_cmd << "grid " << this->OpacityScale->GetWidgetName() << options 
             << " -column " << col << " -row " << row++ << endl;
    tk_cmd  << "grid " << this->PushButtonSet->GetWidgetName() << options 
           << " -column " << col << " -row " << row++ << endl;
    }
  else
    {
    this->ChangeColorButton->ShowLabelOff();

    this->FontFamilyOptionMenu->GetOptionMenu()->IndicatorOff();
    this->FontFamilyOptionMenu->ShowLabelOff();

    this->StylesCheckButtonSet->ShowLabelOff();

    this->OpacityScale->DisplayLabel("");

    this->PushButtonSet->ShowLabelOff();

    tk_cmd << "grid "
           << this->Label->GetWidgetName() << " "
           << this->ChangeColorButton->GetWidgetName() << " "
           << this->FontFamilyOptionMenu->GetWidgetName() << " "
           << this->StylesCheckButtonSet->GetWidgetName() << " "
           << this->OpacityScale->GetWidgetName() << " "
           << this->PushButtonSet->GetWidgetName()
           << " -row 0 -sticky news -padx 1" << endl;
    }

  tk_cmd << ends;
  this->Script(tk_cmd.str());
  tk_cmd.rdbuf()->freeze(0);

  // Synchronize the label width to align everything

  if (this->LongFormat)
    {
    int nb = 0;
    const char *labels[10];
    labels[nb++] = this->ChangeColorButton->GetLabel()->GetWidgetName();
    labels[nb++] = this->FontFamilyOptionMenu->GetLabel()->GetWidgetName();
    labels[nb++] = this->StylesCheckButtonSet->GetLabel()->GetWidgetName();
    labels[nb++] = this->OpacityScale->GetLabel()->GetWidgetName();
    labels[nb++] = this->PushButtonSet->GetLabel()->GetWidgetName();
    vtkKWTkUtilities::SynchroniseLabelsMaximumWidth(
      this->GetApplication()->GetMainInterp(), nb, labels, "-anchor w");
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::Update()
{
  this->UpdateInterface();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateInterface()
{
  // Update enable state

  this->UpdateEnableState();

  // Update all components

  this->UpdateLabel();
  this->UpdateColorButton();
  this->UpdateFontFamilyOptionMenu();
  this->UpdateStylesCheckButtonSet();
  this->UpdateOpacityScale();
  this->UpdatePushButtonSet();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetTextProperty(vtkTextProperty *_arg)
{
  if (this->TextProperty == _arg)
    {
    return;
    }

  this->TextProperty = _arg;
  
  if (this->TextProperty)
    {
    this->UpdateInterface();
    }

  this->Modified();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetActor2D(vtkActor2D *_arg)
{
  if (this->Actor2D == _arg)
    {
    return;
    }

  this->Actor2D = _arg;
  
  if (this->Actor2D)
    {
    this->UpdateInterface();
    }

  this->Modified();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetLongFormat(int _arg)
{
  if (this->LongFormat == _arg)
    {
    return;
    }
  this->LongFormat = _arg;
  this->Modified();

  this->Pack();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShowLabel(int _arg)
{
  if (this->ShowLabel == _arg)
    {
    return;
    }
  this->ShowLabel = _arg;
  this->Modified();

  this->UpdateLabel();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetLabelOnTop(int _arg)
{
  if (this->LabelOnTop == _arg)
    {
    return;
    }
  this->LabelOnTop = _arg;
  this->Modified();

  this->Pack();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateLabel()
{
  if (this->IsCreated() && this->Label)
    {
    this->Script("grid %s %s",
                 (this->ShowLabel ? "" : "remove"), 
                 this->Label->GetWidgetName());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShowColor(int _arg)
{
  if (this->ShowColor == _arg)
    {
    return;
    }
  this->ShowColor = _arg;
  this->Modified();

  this->UpdateColorButton();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetColor(double r, double g, double b) 
{
  double *rgb = this->GetColor();
  if (rgb[0] == r && rgb[1] == g && rgb[2] == b)
    {
    return;
    }

  if (this->TextProperty)
    {
    this->TextProperty->SetColor(r, g, b);
    }
  else if (this->Actor2D && this->Actor2D->GetProperty())
    {
    this->Actor2D->GetProperty()->SetColor(r, g, b);
    }

  this->UpdateColorButton();

  this->AddTraceEntry("$kw(%s) SetColor %lf %lf %lf", this->GetTclName(), r,g,b);

  if (this->ColorChangedCommand)
    {
    this->Script("eval %s", this->ColorChangedCommand);
    }

  if (this->ChangedCommand)
    {
    this->Script("eval %s", this->ChangedCommand);
    }
}

// ----------------------------------------------------------------------------
double* vtkKWTextProperty::GetColor() 
{
  int use_actor_color = 0;
  if (!this->TextProperty)
    {
    use_actor_color = 1;
    }
  else
    {
    // This test is done to maintain backward compatibility (see
    // vtkOpenGL...TextMapper). The default vtkTextProperty color is
    // -1, -1, -1 so that the mappers know that they have to use
    // the actor's color instead.
    double *rgb = this->TextProperty->GetColor();
    if (rgb[0] < 0.0 && rgb[1] < 0.0 && rgb[2] < 0.0)
      {
      use_actor_color = 1;
      }
    }

  if (use_actor_color && this->Actor2D && this->Actor2D->GetProperty())
    {
    return this->Actor2D->GetProperty()->GetColor();
    }
  else if (this->TextProperty)
    {
    return this->TextProperty->GetColor();
    }
  return 0;
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateColorButton()
{
  if (this->IsCreated() && this->ChangeColorButton)
    {
    if (this->GetColor())
      {
      this->ChangeColorButton->SetColor(this->GetColor());
      }

    this->Script("grid %s %s",
                 (this->ShowColor ? "" : "remove"), 
                 this->ChangeColorButton->GetWidgetName());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::ChangeColorButtonCallback(double r, double g, double b) 
{
  this->SetColor(r, g, b);
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShowFontFamily(int _arg)
{
  if (this->ShowFontFamily == _arg)
    {
    return;
    }
  this->ShowFontFamily = _arg;
  this->Modified();

  this->UpdateFontFamilyOptionMenu();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetFontFamily(int v) 
{
  if (this->TextProperty)
    {
    if (this->TextProperty->GetFontFamily() == v)
      {
      return;
      }
    this->TextProperty->SetFontFamily(v);
    }

  this->UpdateFontFamilyOptionMenu();

  this->AddTraceEntry("$kw(%s) SetFontFamily %d", this->GetTclName(), v);

  if (this->ChangedCommand)
    {
    this->Script("eval %s", this->ChangedCommand);
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateFontFamilyOptionMenu()
{
  if (this->IsCreated() &&
      this->FontFamilyOptionMenu &&
      this->TextProperty)
    {
    switch (this->TextProperty->GetFontFamily())
      {
      case VTK_ARIAL:
        this->FontFamilyOptionMenu->GetOptionMenu()->SetValue(
          VTK_KW_TEXT_PROP_ARIAL);
        break;
      case VTK_COURIER:
        this->FontFamilyOptionMenu->GetOptionMenu()->SetValue(
          VTK_KW_TEXT_PROP_COURIER);
        break;
      case VTK_TIMES:
        this->FontFamilyOptionMenu->GetOptionMenu()->SetValue(
          VTK_KW_TEXT_PROP_TIMES);
        break;
      }
    this->Script("grid %s %s",
                 (this->ShowFontFamily ? "" : "remove"), 
                 this->FontFamilyOptionMenu->GetWidgetName());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::FontFamilyCallback() 
{
  if (this->FontFamilyOptionMenu->IsCreated())
    {
    const char *value = this->FontFamilyOptionMenu->GetOptionMenu()->GetValue();
    if (!strcmp(value, VTK_KW_TEXT_PROP_ARIAL))
      {
      this->SetFontFamily(VTK_ARIAL);
      }
    else if (!strcmp(value, VTK_KW_TEXT_PROP_COURIER))
      {
      this->SetFontFamily(VTK_COURIER);
      }
    else if (!strcmp(value, VTK_KW_TEXT_PROP_TIMES))
      {
      this->SetFontFamily(VTK_TIMES);
      }
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShowStyles(int _arg)
{
  if (this->ShowStyles == _arg)
    {
    return;
    }
  this->ShowStyles = _arg;
  this->Modified();

  this->UpdateStylesCheckButtonSet();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateStylesCheckButtonSet()
{
  if (this->IsCreated() && this->StylesCheckButtonSet)
    {
    this->Script("grid %s %s",
                 (this->ShowStyles ? "" : "remove"), 
                 this->StylesCheckButtonSet->GetWidgetName());
    }

  this->UpdateBoldCheckButton();
  this->UpdateItalicCheckButton();
  this->UpdateShadowCheckButton();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetBold(int v) 
{
  if (this->TextProperty)
    {
    if (this->TextProperty->GetBold() == v)
      {
      return;
      }
    this->TextProperty->SetBold(v);
    }

  this->UpdateBoldCheckButton();

  this->AddTraceEntry("$kw(%s) SetBold %d", this->GetTclName(), v);

  if (this->ChangedCommand)
    {
    this->Script("eval %s", this->ChangedCommand);
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateBoldCheckButton()
{
  if (this->IsCreated() && 
      this->StylesCheckButtonSet &&
      this->TextProperty)
    {
    this->StylesCheckButtonSet->GetCheckButtonSet()->SetButtonState(
      VTK_KW_TEXT_PROP_BOLD_ID, this->TextProperty->GetBold());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::BoldCallback() 
{
  if (this->IsCreated())
    {
    this->SetBold(this->StylesCheckButtonSet->GetCheckButtonSet()
                  ->IsButtonSelected(VTK_KW_TEXT_PROP_BOLD_ID));
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetItalic(int v) 
{
  if (this->TextProperty)
    {
    if (this->TextProperty->GetItalic() == v)
      {
      return;
      }
    this->TextProperty->SetItalic(v);
    }

  this->UpdateItalicCheckButton();

  this->AddTraceEntry("$kw(%s) SetItalic %d", this->GetTclName(), v);

  if (this->ChangedCommand)
    {
    this->Script("eval %s", this->ChangedCommand);
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateItalicCheckButton()
{
  if (this->IsCreated() && 
      this->StylesCheckButtonSet &&
      this->TextProperty)
    {
    this->StylesCheckButtonSet->GetCheckButtonSet()->SetButtonState(
      VTK_KW_TEXT_PROP_ITALIC_ID, this->TextProperty->GetItalic());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::ItalicCallback() 
{
  if (this->IsCreated())
    {
    this->SetItalic(this->StylesCheckButtonSet->GetCheckButtonSet()
                    ->IsButtonSelected(VTK_KW_TEXT_PROP_ITALIC_ID));
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShadow(int v) 
{
  if (this->TextProperty)
    {
    if (this->TextProperty->GetShadow() == v)
      {
      return;
      }
    this->TextProperty->SetShadow(v);
    }

  this->UpdateShadowCheckButton();

  this->AddTraceEntry("$kw(%s) SetShadow %d", this->GetTclName(), v);

  if (this->ChangedCommand)
    {
    this->Script("eval %s", this->ChangedCommand);
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateShadowCheckButton()
{
  if (this->IsCreated() && 
      this->StylesCheckButtonSet &&
      this->TextProperty)
    {
    this->StylesCheckButtonSet->GetCheckButtonSet()->SetButtonState(
      VTK_KW_TEXT_PROP_SHADOW_ID, this->TextProperty->GetShadow());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::ShadowCallback() 
{
  if (this->IsCreated())
    {
    this->SetShadow(this->StylesCheckButtonSet->GetCheckButtonSet()
                    ->IsButtonSelected(VTK_KW_TEXT_PROP_SHADOW_ID));
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShowOpacity(int _arg)
{
  if (this->ShowOpacity == _arg)
    {
    return;
    }
  this->ShowOpacity = _arg;
  this->Modified();

  this->UpdateOpacityScale();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetOpacityNoTrace(float v) 
{
  if (this->GetOpacity() == v)
    {
    return;
    }

  if (this->TextProperty)
    {
    this->TextProperty->SetOpacity(v);
    }
  else if (this->Actor2D && this->Actor2D->GetProperty())
    {
    this->Actor2D->GetProperty()->SetOpacity(v);
    }

  this->UpdateOpacityScale();

  if (this->ChangedCommand)
    {
    this->Script("eval %s", this->ChangedCommand);
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetOpacity(float v) 
{
  this->SetOpacityNoTrace(v);
  if ( !this->IsCreated() )
    {
    return;
    }
  this->AddTraceEntry("$kw(%s) SetOpacity %f", this->GetTclName(), v);
}

// ----------------------------------------------------------------------------
float vtkKWTextProperty::GetOpacity() 
{
  int use_actor_opacity = 0;
  if (!this->TextProperty)
    {
    use_actor_opacity = 1;
    }
  else
    {
    // This test is done to maintain backward compatibility (see
    // vtkOpenGL...TextMapper). The default vtkTextProperty opacity is
    // -1 so that the mappers know that they have to use
    // the actor's opacity instead.
    if (this->TextProperty->GetOpacity() < 0.0)
      {
      use_actor_opacity = 1;
      }
    }

  if (use_actor_opacity && this->Actor2D && this->Actor2D->GetProperty())
    {
    return this->Actor2D->GetProperty()->GetOpacity();
    }
  else if (this->TextProperty)
    {
    return this->TextProperty->GetOpacity();
    }
  return 0.0;
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateOpacityScale()
{
  if (this->IsCreated() && 
      this->OpacityScale &&
      this->TextProperty)
    {
    this->OpacityScale->SetValue(this->GetOpacity());
    this->Script("grid %s %s",
                 (this->ShowOpacity ? "" : "remove"), 
                 this->OpacityScale->GetWidgetName());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::OpacityCallback() 
{
  if (this->OpacityScale->IsCreated())
    {
    this->SetOpacityNoTrace(this->OpacityScale->GetValue());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::OpacityEndCallback() 
{
  if (this->OpacityScale->IsCreated())
    {
    this->SetOpacity(this->OpacityScale->GetValue());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShowCopy(int _arg)
{
  if (this->ShowCopy == _arg)
    {
    return;
    }
  this->ShowCopy = _arg;
  this->Modified();

  this->UpdatePushButtonSet();
}

// ----------------------------------------------------------------------------
vtkKWPushButton* vtkKWTextProperty::GetCopyButton()
{
  if (this->IsCreated())
    {
    return this->PushButtonSet->GetPushButtonSet()->GetButton(
      VTK_KW_TEXT_PROP_COPY_ID);
    }
  return NULL;
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdatePushButtonSet()
{
  if (this->IsCreated() && this->PushButtonSet)
    {
    this->PushButtonSet->GetPushButtonSet()->SetButtonVisibility(
      VTK_KW_TEXT_PROP_COPY_ID, this->ShowCopy);

    this->Script("grid %s %s",
                 (this->PushButtonSet->GetPushButtonSet()
                  ->GetNumberOfVisibleButtons() ? "" : "remove"), 
                 this->PushButtonSet->GetWidgetName());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::CopyValuesFrom(vtkKWTextProperty *widget)
{
  if (widget)
    {
    vtkTextProperty *tprop = widget->GetTextProperty();
    if (tprop)
      {
      this->SetColor(widget->GetColor());
      this->SetFontFamily(tprop->GetFontFamily());
      this->SetBold(tprop->GetBold());
      this->SetItalic(tprop->GetItalic());
      this->SetShadow(tprop->GetShadow());
      this->SetOpacity(widget->GetOpacity());
      }
    }
}

//----------------------------------------------------------------------------
void vtkKWTextProperty::SaveInTclScript(ofstream *file, 
                                        const char *tcl_name, int tabify)
{
  static int instance_count = 0;

  const char *name;
  char buffer[128];
  if (tcl_name)
    {
    name = tcl_name;
    }
  else
    {
    sprintf(buffer, "TextProperty%d", instance_count++);
    name = buffer;
    *file << "vtkTextProperty " << name << endl;
    }

  vtkTextProperty *tprop = this->TextProperty;

  if (tprop)
    {
    double *rgb = this->GetColor();
    if (tabify)
      {
      *file << "\t";
      }
    *file << name << " SetColor " << rgb[0] << " "  << rgb[1] << " "  << rgb[2]
          << endl;
    if (tabify)
      {
      *file << "\t";
      }
    *file << name << " SetFontFamily " << tprop->GetFontFamily() << endl;
    if (tabify)
      {
      *file << "\t";
      }
    *file << name << " SetBold " << tprop->GetBold() << endl;
    if (tabify)
      {
      *file << "\t";
      }
    *file << name << " SetItalic " << tprop->GetItalic() << endl;
    if (tabify)
      {
      *file << "\t";
      }
    *file << name << " SetShadow " << tprop->GetShadow() << endl;
    if (tabify)
      {
      *file << "\t";
      }
    *file << name << " SetOpacity " << this->GetOpacity() << endl;
    }
}

//----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateEnableState()
{
  this->Superclass::UpdateEnableState();


  this->PropagateEnableState(this->Label);
  this->PropagateEnableState(this->ChangeColorButton);
  this->PropagateEnableState(this->FontFamilyOptionMenu);
  this->PropagateEnableState(this->StylesCheckButtonSet);
  this->PropagateEnableState(this->OpacityScale);
  this->PropagateEnableState(this->PushButtonSet);
}

//----------------------------------------------------------------------------
void vtkKWTextProperty::SetChangedCommand(
  vtkKWObject *object, const char *method)
{
  this->SetObjectMethodCommand(&this->ChangedCommand, object, method);
}

//----------------------------------------------------------------------------
void vtkKWTextProperty::SetColorChangedCommand(
  vtkKWObject *object, const char *method)
{
  this->SetObjectMethodCommand(&this->ColorChangedCommand, object, method);
}

//----------------------------------------------------------------------------
void vtkKWTextProperty::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  if (this->TextProperty)
    {
    os << indent << "TextProperty:\n";
    this->TextProperty->PrintSelf(os, indent.GetNextIndent());
    }
  else
    {
    os << indent << "TextProperty: None" << endl;
    }
  if (this->Actor2D)
    {
    os << indent << "Actor2D:\n";
    this->Actor2D->PrintSelf(os, indent.GetNextIndent());
    }
  else
    {
    os << indent << "Actor2D: None" << endl;
    }
  os << indent << "LongFormat: " << (this->LongFormat ? "On" : "Off") << endl;
  os << indent << "ShowLabel: " << (this->ShowLabel ? "On" : "Off") << endl;
  os << indent << "LabelOnTop: " << (this->LabelOnTop ? "On" : "Off") << endl;
  os << indent << "ShowColor: " << (this->ShowColor ? "On" : "Off") << endl;
  os << indent << "ShowFontFamily: " 
     << (this->ShowFontFamily ? "On" : "Off") << endl;
  os << indent << "ShowStyles: " 
     << (this->ShowStyles ? "On" : "Off") << endl;
  os << indent << "ShowOpacity: " 
     << (this->ShowOpacity ? "On" : "Off") << endl;
  os << indent << "ShowCopy: " << (this->ShowCopy ? "On" : "Off") << endl;
  os << indent << "ChangedCommand: " 
     << (this->ChangedCommand ? this->ChangedCommand : "None") << endl;
  os << indent << "ColorChangedCommand: " 
     << (this->ColorChangedCommand ? this->ColorChangedCommand : "None") << endl;
  os << indent << "Label: " << this->Label << endl;
}

