set projdir [get_property DIRECTORY [current_project]]
set projname [get_property NAME [current_project]]

file copy -force $projdir/$projname.runs/impl_1/[get_property top [current_fileset]].bit $projdir/$projname.bit
file copy -force [get_files -of [current_fileset] *.hwh] $projdir/$projname.hwh


# write_bd_tcl [get_property DIRECTORY [current_project]]/[get_property NAME [current_project]].tcl
# file copy [get_property DIRECTORY [current_run]]/design_1_wrapper.bit [get_property DIRECTORY [current_project]]/[get_property NAME [current_project]].bit
# file copy [get_files -of [get_filesets sources_1] *.hwh] [get_property DIRECTORY [current_project]]/[get_property NAME [current_project]].hwh

