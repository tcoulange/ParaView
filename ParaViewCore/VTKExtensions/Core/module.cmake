vtk_module(vtkPVVTKExtensionsCore
  GROUPS
    ParaViewCore
  DEPENDS
    vtkFiltersCore
    vtkParallelCore
    vtkPVCommon
  PRIVATE_DEPENDS
    vtksys
)
