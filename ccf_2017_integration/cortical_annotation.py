import os
import subprocess

#
# Directory paths
#
working_directory = '/data/ccf/2017_integration/streamlines'
cc_directory = '/allen/programs/celltypes/production/0378/informatics/model/P56/corticalCoordinates/'
script_directory = '/data/ccf/cortical_coordinates/full/code/'
bin_directory = '/data/informatics/lydian/CCF/Q22014/CorticalCoordinates/build/bin'
new_directory = os.path.join( '2017' )


# create an isocortex mask for all views
script_path = os.path.join( script_directory, 'scripts','transfer_surface_to_views.py' )
input_file = os.path.join( new_directory, 'isocortex_mask_10.nrrd' )
output_directory = os.path.join(working_directory, 'isocortex_mask')
cmd = ['python', script_path, input_file, output_directory]
if 0:
	print cmd
	subprocess.call(cmd)
	
# project the flatmap view back onto the 3d volume
script_path = os.path.join( script_directory, 'transfer_view_to_surface.py' )
view = 'flatmap_dorsal'
input_file = os.path.join(working_directory, 'isocortex_mask', '%s.nrrd' % view )
lookup_file = os.path.join(working_directory, 'view_lookup', '%s.h5' % view )
output_file = os.path.join( working_directory, 'temp', 'volume.nrrd' )
cmd = ['python', script_path, input_file, output_file, lookup_file]
if 0:
	print cmd
	subprocess.call(cmd)

# project from surface down into isocortex volume
script_path = os.path.join( script_directory, 'simple_3d_fill.py' )
input_file = os.path.join( working_directory, 'temp', 'volume.nrrd' )
lookup_file = os.path.join(working_directory, '2017', 'streamline_metrics', 'closest_surface_voxel_index.nrrd' )
output_file = os.path.join( working_directory, 'temp', 'filled.nrrd' )
cmd = ['python', script_path, input_file, output_file, lookup_file]
if 0:
	print cmd
	subprocess.call(cmd)
	
# make layer 4 only volume
script_path = os.path.join( script_directory, 'redo_cortex','binary_threshold_image.py' )
input_file = os.path.join( new_directory, 'isocortex_layers_10.nrrd' )
output_file  = os.path.join(working_directory, 'layer_4_mask', 'layer_4_mask.nrrd')
value = '1121'
cmd = ['python', script_path, input_file, output_file, value, value, '255', '0']
if 0:
	print cmd
	subprocess.call(cmd)


# make layer 4 views
script_path = os.path.join( script_directory, 'scripts','create_max_projection_views.py' )
input_file = os.path.join( working_directory, 'layer_4_mask', 'layer_4_mask.nrrd' )
output_directory = os.path.join(working_directory, 'layer_4_mask')
cmd = ['python', script_path, input_file, output_directory]
if 0:
	print cmd
	subprocess.call(cmd)

	
# create views from 2016 master
script_path = os.path.join( script_directory, 'scripts','transfer_view_to_surface.py' )
input_file = os.path.join( '/allen/aibs/ccf/cortical_coordinates/master_on_full/master/master.nrrd' )
output_directory = os.path.join(working_directory, '2016_master')
cmd = ['python', script_path, input_file, output_directory]
if 0:
	print cmd
	subprocess.call(cmd)
	
script_path = os.path.join( script_directory, 'simple_3d_fill.py' )
input_file = os.path.join( '/allen/aibs/ccf/cortical_coordinates/master_on_full/master/master.nrrd' )
lookup_file = os.path.join(working_directory, '2017', 'streamline_metrics', 'closest_surface_voxel_index.nrrd' )
output_file = os.path.join( working_directory, '2016_master', 'filled.nrrd' )
cmd = ['python', script_path, input_file, output_file, lookup_file]
if 0:
	print cmd
	subprocess.call(cmd)

	
# transfer cleaned annotation to master
script_path = os.path.join( script_directory, 'transfer_view_to_surface.py' )
view = 'flatmap_dorsal'
input_file = os.path.join(working_directory, '2017_cleaned', '%s_cleaned.nrrd' % view )
lookup_file = os.path.join(working_directory, 'view_lookup', '%s.h5' % view )
output_file = os.path.join( working_directory, '2017_cleaned', 'master.nrrd' )
append_file = os.path.join( '/allen/aibs/ccf/cortical_coordinates/master_on_full/master/master.nrrd' )
cmd = ['python', script_path, input_file, output_file, lookup_file, append_file ]
if 0:
	print cmd
	subprocess.call(cmd)

# create views from 2017 cleaned master
script_path = os.path.join( script_directory, 'scripts','create_max_projection_views.py' )
input_file = os.path.join( working_directory, '2017_cleaned', 'master.nrrd' )
output_directory = os.path.join(working_directory, '2017_cleaned')
cmd = ['python', script_path, input_file, output_directory]
if 0:
	print cmd
	subprocess.call(cmd)

script_path = os.path.join( script_directory, 'simple_3d_fill.py' )
input_file = os.path.join( working_directory, '2017_cleaned', 'master.nrrd' )
lookup_file = os.path.join(working_directory, '2017', 'streamline_metrics', 'closest_surface_voxel_index.nrrd' )
output_file = os.path.join( working_directory, '2017_cleaned', 'filled.nrrd' )
cmd = ['python', script_path, input_file, output_file, lookup_file]
if 0:
	print cmd
	subprocess.call(cmd)

#################################
##
## Clean and fill views
##
##################################

def view_fill( view_dir, origin_dir, view ) :


	do_clean_up = 1

	edited_file = os.path.join(working_directory, origin_dir, '%s_edited.nrrd' % view )
	
	if not os.path.isfile(edited_file):
	
		# fill in missing labels by neighborhood voting
		script_path = os.path.join( script_directory, 'neighborhood_vote_fill.py' )
		input_file = os.path.join(working_directory, origin_dir, '%s.nrrd' % view )
		mask_file = os.path.join(working_directory, 'isocortex_mask', '%s.nrrd' % view )
		output_file = os.path.join(working_directory, view_dir, '%s_filled.nrrd' % view )
		cmd = ['python', script_path, input_file, mask_file, output_file ]
		if do_clean_up:
			print cmd
			subprocess.call(cmd)
			
			
	input_file = ''
	if not os.path.isfile(edited_file):
		input_file = os.path.join(working_directory, view_dir, '%s_filled.nrrd' % view )
	else :
		input_file = edited_file

	# concatenate with 2017_front with 2017_clean master
	script_path = os.path.join( script_directory, 'transfer_view_to_surface.py' )
	lookup_file = os.path.join(working_directory, 'view_lookup', '%s.h5' % view )
	output_file = os.path.join( working_directory, view_dir, 'master.nrrd' )
	append_file = os.path.join( working_directory, origin_dir, 'master.nrrd' )
	cmd = ['python', script_path, input_file, output_file, lookup_file, append_file ]

	if do_clean_up:
		print cmd
		subprocess.call(cmd)

	# create views from 2016_front master
	script_path = os.path.join( script_directory, 'scripts','transfer_surface_to_views.py' )
	input_file = os.path.join( working_directory, view_dir, 'master.nrrd' )
	output_directory = os.path.join(working_directory, view_dir )
	cmd = ['python', script_path, input_file, output_directory]
	if do_clean_up:
		print cmd
		subprocess.call(cmd)

def make_fill( view_dir ) :
	script_path = os.path.join( script_directory, 'simple_3d_fill.py' )
	input_file = os.path.join( working_directory, view_dir, 'master.nrrd' )
	lookup_file = os.path.join(working_directory, '2017', 'streamline_metrics', 'closest_surface_voxel_index.nrrd' )
	output_file = os.path.join( working_directory, view_dir, 'filled.nrrd' )
	cmd = ['python', script_path, input_file, output_file, lookup_file]
	if 1 :
		print cmd
		subprocess.call(cmd)
		
# work on flatmap_dorsal view
view_dir = '2017_iterate'
origin_dir = '2017_cleaned'
view = 'flatmap_dorsal'
if 0:
	view_fill( view_dir, origin_dir, view )

# work on top view
view_dir = '2017_iterate'
origin_dir = '2017_iterate'
view = 'top'
if 0:
	view_fill( view_dir, origin_dir, view )
	
# work on side view
view_dir = '2017_iterate'
origin_dir = '2017_iterate'
view = 'side'
if 0:
	view_fill( view_dir, origin_dir, view )

# work on rotated view
view_dir = '2017_iterate'
origin_dir = '2017_iterate'
view = 'rotated'
if 0:
	view_fill( view_dir, origin_dir, view )
	
# work on back view
view_dir = '2017_iterate'
origin_dir = '2017_iterate'
view = 'back'
if 0:
	view_fill( view_dir, origin_dir, view )

# work on medial view
view_dir = '2017_iterate'
origin_dir = '2017_iterate'
view = 'medial'
if 0:
	view_fill( view_dir, origin_dir, view )
	
# work on front view
view_dir = '2017_iterate'
origin_dir = '2017_iterate'
view = 'front'
if 0:
	view_fill( view_dir, origin_dir, view )
	
# work on bottom view
view_dir = '2017_iterate'
origin_dir = '2017_iterate'
view = 'bottom'
if 0:
	view_fill( view_dir, origin_dir, view )
	
# filter out non surface voxel from master

origin_dir = '2017_iterate'
view_dir = '2017_final'

script_path = os.path.join( script_directory, 'mask_volume.py' )
input_file = os.path.join(working_directory, origin_dir, 'master.nrrd' )
mask_file = os.path.join(working_directory, '2017', 'isocortex_top_surface_10.nrrd' )
output_file = os.path.join(working_directory, view_dir, 'master_prep.nrrd' )
cmd = ['python', script_path, input_file, mask_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)


# are ther any surface voxel that is not annotated?
# iterate and fill in
script_path = os.path.join( script_directory, 'redo_cortex', 'list_unlabelled_surface_voxels.py' )
input_file = os.path.join(working_directory, view_dir, 'master.nrrd' )
mask_file = os.path.join(working_directory, '2017', 'isocortex_top_surface_10.nrrd' )
cmd = ['python', script_path, input_file, mask_file ]
if 0:
	print cmd
	subprocess.call(cmd)

if 0:
	make_fill( view_dir )

# use the annotation filled volume to mask the isocortex mask

script_path = os.path.join( script_directory, 'not_volume.py' )
input_file = os.path.join(working_directory, view_dir, 'filled.nrrd' )
output_file = os.path.join(working_directory, view_dir, 'not_mask.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)


script_path = os.path.join( script_directory, 'mask_volume.py' )
input_file = os.path.join(working_directory, '2017', 'isocortex_mask_10.nrrd' )
mask_file = os.path.join(working_directory, view_dir, 'not_mask.nrrd' )
output_file = os.path.join(working_directory, view_dir, 'volumen_unlabelled.nrrd' )
cmd = ['python', script_path, input_file, mask_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
# filled in missing voxels by FRP
script_path = os.path.join( script_directory, 'remap_value.py' )
input_file = os.path.join(working_directory, view_dir, 'volumen_unlabelled.nrrd' )
output_file = os.path.join(working_directory, view_dir, 'value_remapped.nrrd' )
cmd = ['python', script_path, input_file, output_file, '2', '395' ]
if 0:
	print cmd
	subprocess.call(cmd)

script_path = os.path.join( script_directory, 'overwrite_image.py' )
input1_file = os.path.join( working_directory, view_dir, 'value_remapped.nrrd' )
input2_file = os.path.join( working_directory, view_dir, 'filled.nrrd' )
output_file = os.path.join( working_directory, view_dir, 'isocortex_annotation_10.nrrd' )
cmd = ['python', script_path, input1_file, input2_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)

# make half volume version
script_path = os.path.join( script_directory, 'permute_and_crop_data.py' )
input_file = os.path.join( working_directory, view_dir, 'isocortex_annotation_10.nrrd' )
output_file = os.path.join( working_directory, view_dir, 'isocortex_annotation_asp.nrrd' )
reference_file = '/data/ccf/cortical_coordinates/full/average_template/avg_template_10x10x10_half.nii.gz'
flip_flag = '1'
cmd = ['python', script_path, input_file, reference_file, output_file, flip_flag]
if 0:
	print cmd
	subprocess.call(cmd)


# make final views
view_dir = '2017_final/views'
origin_dir = '2017_final'

script_path = os.path.join( script_directory, 'scripts','transfer_surface_to_views.py' )
input_file = os.path.join( working_directory, origin_dir, 'master.nrrd' )
output_directory = os.path.join(working_directory, view_dir )
cmd = ['python', script_path, input_file, output_directory]
if 1:
	print cmd
	subprocess.call(cmd)




	
