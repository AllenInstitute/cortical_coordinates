import os
import subprocess

#
# Directory paths
#
working_directory = '/data/ccf/2017_integration/streamlines'
cc_directory = '/allen/programs/celltypes/production/0378/informatics/model/P56/corticalCoordinates/'
script_directory = '/data/ccf/cortical_coordinates/full/code/'
bin_directory = '/data/informatics/lydian/CCF/Q22014/CorticalCoordinates/build/bin'


####################################
#
# Verify isocortex mask
#
####################################

old_directory = os.path.join( '2016' )
new_directory = os.path.join( '2017' )

dir_list = [old_directory, new_directory]
for d in dir_list :
	if not os.path.exists(d):
		os.makedirs(d)
		
# convert 2016 full space files into half hemisphere files
script_path = os.path.join( script_directory, 'permute_and_crop_data.py' )
input_file = os.path.join( cc_directory, 'isocortex_mask_10.nrrd' )
output_file = os.path.join( old_directory, 'isocortex_mask_asp.nrrd' )
reference_file = '/data/ccf/cortical_coordinates/full/average_template/avg_template_10x10x10_half.nii.gz'
flip_flag = '1'
cmd = ['python', script_path, input_file, reference_file, output_file, flip_flag]
if 0:
	print cmd
	subprocess.call(cmd)

# null out data in the other hemisphere 
script_path = os.path.join( script_directory, 'interpolate', 'clean_hemisphere_half.py' )
input_file = os.path.join( old_directory, 'isocortex_mask_asp.nrrd' )
output_file = os.path.join( old_directory, 'isocortex_mask_asp.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
# convert 2016 full space files into half hemisphere files
script_path = os.path.join( script_directory, 'permute_and_crop_data.py' )
input_file = os.path.join( cc_directory, 'isocortex_boundary_10.nrrd' )
output_file = os.path.join( old_directory, 'isocortex_boundary_asp.nrrd' )
reference_file = '/data/ccf/cortical_coordinates/full/average_template/avg_template_10x10x10_half.nii.gz'
flip_flag = '1'
cmd = ['python', script_path, input_file, reference_file, output_file, flip_flag]
if 0:
	print cmd
	subprocess.call(cmd)

# null out data in the other hemisphere 
script_path = os.path.join( script_directory, 'interpolate', 'clean_hemisphere_half.py' )
input_file = os.path.join( old_directory, 'isocortex_boundary_asp.nrrd' )
output_file = os.path.join( old_directory, 'isocortex_boundary_asp.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)	
	
# convert 2017 mask into nrrd format
script_path = os.path.join( script_directory, 'read_write_image.py' )
input_file = '/data/ccf/June2017Deliverables/CTX-block.nii.gz'
output_file = os.path.join( new_directory, 'isocortex_mask_asp.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
script_path = os.path.join( script_directory, 'remap_value.py' )
input_file = os.path.join( new_directory, 'isocortex_mask_asp.nrrd' )
output_file = os.path.join( new_directory, 'isocortex_mask_asp.nrrd' )
old_value = '255'
new_value = '2'
cmd = ['python', script_path, input_file, output_file, old_value, new_value ]
if 0:
	print cmd
	subprocess.call(cmd)	

# diff between 2016 and 2017 masks
script_path = os.path.join( script_directory, 'interpolate', 'diff_annotation.py' )
input1_file = os.path.join( old_directory, 'isocortex_mask_asp.nrrd' )
input2_file = os.path.join( new_directory, 'isocortex_mask_asp.nrrd' )
output_file = os.path.join( new_directory, 'diff_isocortex_mask_asp.nrrd' )
cmd = ['python', script_path, input1_file, input2_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
#####################################
#
# Result: 
# - there have been a modification of the isocortex mask beyond additional posterior area
# - there have been pixel level changes near the white matter surface
# - at the anterior part of the brain there has been pixel level changes to the pia surface
# - majority of the isocortex is the same as what was published in 2017
#
#####################################	

####################################
# 
# Verify that the layer volume is a complete overlap
#
####################################

# convert 2017 mask into nrrd format
script_path = os.path.join( script_directory, 'read_write_image.py' )
input_file = '/data/ccf/June2017Deliverables/CTX-layers.nii.gz'
output_file = os.path.join( new_directory, 'isocortex_layers_asp.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
# combined all layers
script_path = os.path.join( script_directory, 'not_volume.py' )
input_file = os.path.join( new_directory, 'isocortex_layers_asp.nrrd' )
output_file = os.path.join( new_directory, 'combined_layers_asp.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)	
	
script_path = os.path.join( script_directory, 'not_volume.py' )
input_file = os.path.join( new_directory, 'combined_layers_asp.nrrd' )
output_file = os.path.join( new_directory, 'combined_layers_asp.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)	

script_path = os.path.join( script_directory, 'remap_value.py' )
input_file = os.path.join( new_directory, 'combined_layers_asp.nrrd' )
output_file = os.path.join( new_directory, 'combined_layers_asp.nrrd' )
old_value = '1'
new_value = '2'
cmd = ['python', script_path, input_file, output_file, old_value, new_value ]
if 0:
	print cmd
	subprocess.call(cmd)

# diff between 2017 isocortex and combined layer masks
script_path = os.path.join( script_directory, 'interpolate', 'diff_annotation.py' )
input1_file = os.path.join( new_directory, 'isocortex_mask_asp.nrrd' )
input2_file = os.path.join( new_directory, 'combined_layers_asp.nrrd' )
output_file = os.path.join( new_directory, 'diff_mask_layer_asp.nrrd' )
cmd = ['python', script_path, input1_file, input2_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
	
####################################
# 
# Result:
# There are 4 voxels which don't overlap
# Delete these 4 voxels from isocortex_mask
#
####################################

# scrub out the 4 voxels from isocortex_mask
script_path = os.path.join( script_directory, 'mask_volume.py' )
input_file = os.path.join( new_directory, 'isocortex_mask_asp.nrrd' )
mask_file = os.path.join( new_directory, 'combined_layers_asp.nrrd' )
output_file = os.path.join( new_directory, 'isocortex_mask_asp.nrrd' )
cmd = ['python', script_path, input_file, mask_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)

	
#######################################
#
# Find boundary voxels
#
#######################################
bin_path = os.path.join( bin_directory, 'FindSurfaceVoxels' )
input_file = os.path.join( new_directory, 'isocortex_mask_asp.nrrd' )
output1_file = os.path.join( new_directory, 'surface_output_1.nii.gz' )
output2_file = os.path.join( new_directory, 'surface_output_2.nii.gz' )
cmd = [bin_path, '--foreground', '2', input_file, output1_file, output2_file ]
if 0:
	print cmd
	subprocess.call(cmd)

script_path = os.path.join( script_directory, 'read_write_image.py' )
input_file = os.path.join( new_directory, 'surface_output_2.nii.gz' )
output_file = os.path.join( new_directory, 'surface_output_2.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
script_path = os.path.join( script_directory, 'remap_value.py' )
input_file = os.path.join( new_directory, 'surface_output_2.nrrd' )
output_file = os.path.join( new_directory, 'surface_output_2.nrrd' )
old_value = '1'
new_value = '4'
cmd = ['python', script_path, input_file, output_file, old_value, new_value ]
if 0:
	print cmd
	subprocess.call(cmd)

# autoclassify using the layer volume
script_path = os.path.join( script_directory, 'redo_cortex', 'auto_classify_boundary.py' )
input_file = os.path.join( new_directory, 'surface_output_2.nrrd' )
mask_file = os.path.join( new_directory, 'isocortex_layers_asp.nrrd' )
output_file = os.path.join( new_directory, 'surface_auto_classified.nrrd' )
cmd = ['python', script_path, input_file, mask_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)

#
# using ITK-SNAP to manually adjust boundaries
#
# 1 = pia boundary
# 3 = white matter boundary
# 4 = tangential boundary
#

# count layer type for each boundary type
script_path = os.path.join( script_directory, 'redo_cortex', 'count_boundary_types.py' )
input_file = os.path.join( new_directory, 'surface_manual_corrected.nrrd' )
mask_file = os.path.join( new_directory, 'isocortex_layers_asp.nrrd' )
cmd = ['python', script_path, input_file, mask_file ]
if 0:
	print cmd
	subprocess.call(cmd)


# enforce pia boundary to be layer 1
# enforece white matter boundayr to be layer 6a, 6b
script_path = os.path.join( script_directory, 'redo_cortex', 'enforced_classify_boundary.py' )
input_file = os.path.join( new_directory, 'surface_manual_corrected.nrrd' )
mask_file = os.path.join( new_directory, 'isocortex_layers_asp.nrrd' )
output_file = os.path.join( new_directory, 'surface_enforced_corrected.nrrd' )
cmd = ['python', script_path, input_file, mask_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
script_path = os.path.join( script_directory, 'redo_cortex', 'count_boundary_types.py' )
input_file = os.path.join( new_directory, 'surface_enforced_corrected.nrrd' )
mask_file = os.path.join( new_directory, 'isocortex_layers_asp.nrrd' )
cmd = ['python', script_path, input_file, mask_file ]
if 0:
	print cmd
	subprocess.call(cmd)

#
# More editing
#

script_path = os.path.join( script_directory, 'redo_cortex', 'count_boundary_types.py' )
input_file = os.path.join( new_directory, 'surface_enforced_edited.nrrd' )
mask_file = os.path.join( new_directory, 'isocortex_layers_asp.nrrd' )
cmd = ['python', script_path, input_file, mask_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
# enforce pia boundary to be layer 1
# enforece white matter boundayr to be layer 6a, 6b
script_path = os.path.join( script_directory, 'redo_cortex', 'enforced_classify_boundary.py' )
input_file = os.path.join( new_directory, 'surface_enforced_edited.nrrd' )
mask_file = os.path.join( new_directory, 'isocortex_layers_asp.nrrd' )
output_file = os.path.join( new_directory, 'surface_enforced_edited_2.nrrd' )
cmd = ['python', script_path, input_file, mask_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
script_path = os.path.join( script_directory, 'redo_cortex', 'count_boundary_types.py' )
input_file = os.path.join( new_directory, 'surface_enforced_edited_2.nrrd' )
mask_file = os.path.join( new_directory, 'isocortex_layers_asp.nrrd' )
cmd = ['python', script_path, input_file, mask_file ]
if 0:
	print cmd
	subprocess.call(cmd)

#
# Final edited version is surface_enforced_edited_2.nrrd
# rename it as isocortex_boundary_asp
#
script_path = os.path.join( script_directory, 'read_write_image.py' )
input_file = os.path.join( new_directory, 'surface_enforced_edited_2.nrrd' )
output_file = os.path.join( new_directory, 'isocortex_boundary_asp.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
#
# mask with isocortex to remove stray voxels
#
script_path = os.path.join( script_directory, 'mask_volume.py' )
input_file = os.path.join( new_directory, 'isocortex_boundary_asp.nrrd' )
mask_file = os.path.join( new_directory, 'isocortex_mask_asp.nrrd' )
output_file = os.path.join( new_directory, 'isocortex_boundary_asp.nrrd' )
cmd = ['python', script_path, input_file, mask_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)


##############################
#
# Compute the laplacian
#  
##############################

# Compute signed distance from isocortex mask
bin_path = os.path.join( bin_directory, 'SignedDistance' )
input_file = os.path.join( new_directory, 'isocortex_mask_asp.nrrd' )
output_file = os.path.join( new_directory, 'isocortex_signed_distance.nii.gz' )
cmd = [bin_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)

# computed normalized gradient of the signed distance map
bin_path = os.path.join( bin_directory, 'NormalizedGradient' )
input_file = os.path.join( new_directory, 'isocortex_signed_distance.nii.gz' )
output_file = os.path.join( new_directory, 'isocortex_boundary_normal.nrrd' )
cmd = [bin_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
# create mask of isocortex that excludes boundary voxels
bin_path = os.path.join( bin_directory, 'BinaryThresholdImage' )
input_file = os.path.join( new_directory, 'isocortex_boundary_asp.nrrd' )
output_file = os.path.join( new_directory, 'not_boundary_mask.nii.gz' )
cmd = [bin_path, input_file, '0', '0', '1', '0', output_file, 'UINT', '3' ]
if 0:
	print cmd
	subprocess.call(cmd)

bin_path = os.path.join( bin_directory, 'MaskImage' )
input_file = os.path.join( new_directory, 'isocortex_mask_asp.nrrd' )
mask_file = os.path.join( new_directory, 'not_boundary_mask.nii.gz' )
output_file = os.path.join( new_directory, 'exclude_boundary_mask.nii.gz' )
cmd = [bin_path, input_file, mask_file, '0', output_file, 'UINT', '3' ]
if 0:
	print cmd
	subprocess.call(cmd)


# initialize laplacian with a few iterations
bin_path = os.path.join( bin_directory, 'ComputeLaplacian' )
mask_file = os.path.join( new_directory, 'exclude_boundary_mask.nii.gz' )
boundary_file = os.path.join( new_directory, 'isocortex_boundary_asp.nrrd' )
normal_file = os.path.join( new_directory, 'isocortex_boundary_normal.nrrd' )
output_file = os.path.join( new_directory, 'laplacian00.nii.gz' )
num_iterations = str(10)
threshold = str(0.025)
cmd = [bin_path, mask_file, boundary_file, num_iterations, output_file, threshold, normal_file ]
if 0:
	print cmd
	subprocess.call(cmd)

# compute laplacian in batches
num_iterations = str(1000)

#for n in range(10) :
for n in range(10,15) :
	input_file = os.path.join( new_directory, 'laplacian%02d.nii.gz' % (n) )
	output_file = os.path.join( new_directory, 'laplacian%02d.nii.gz' % (n+1) )
	cmd = [bin_path, mask_file, boundary_file, num_iterations, output_file, threshold, normal_file, input_file ]
	if 0:
		print cmd
		subprocess.call(cmd)

		
#
#
# write results into full space for streamline computations
# - mask, boundary, laplacian
#
script_path = os.path.join( script_directory, 'flip_and_shift_volume.py' )
input_file = os.path.join( new_directory, 'isocortex_boundary_asp.nrrd' )
output_file = os.path.join( new_directory, 'isocortex_boundary_10.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
script_path = os.path.join( script_directory, 'flip_and_shift_volume.py' )
input_file = os.path.join( new_directory, 'isocortex_top_surface_asp.nrrd' )
output_file = os.path.join( new_directory, 'isocortex_top_surface_10.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)


script_path = os.path.join( script_directory, 'flip_and_shift_volume.py' )
input_file = os.path.join( new_directory, 'isocortex_mask_asp.nrrd' )
output_file = os.path.join( new_directory, 'isocortex_mask_10.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
script_path = os.path.join( script_directory, 'flip_and_shift_volume.py' )
input_file = os.path.join( new_directory, 'isocortex_layers_asp.nrrd' )
output_file = os.path.join( new_directory, 'isocortex_layers_10.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)


update_laplacian = 0
	
script_path = os.path.join( script_directory, 'read_write_image.py' )
input_file = os.path.join( new_directory, 'laplacian12.nii.gz' )
output_file = os.path.join( new_directory, 'laplacian_asp.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if update_laplacian:
	print cmd
	subprocess.call(cmd)
	
script_path = os.path.join( script_directory, 'flip_and_shift_volume.py' )
input_file = os.path.join( new_directory, 'laplacian_asp.nrrd' )
output_file = os.path.join( new_directory, 'laplacian_10.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if update_laplacian:
	print cmd
	subprocess.call(cmd)

#
# crop the file so that we smaller file sizes
#
script_path = os.path.join( script_directory, 'full_crop_hemisphere.py' )
input_file = os.path.join( new_directory, 'isocortex_boundary_10.nrrd' )
output_file = os.path.join( new_directory, 'isocortex_boundary_crop.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)

script_path = os.path.join( script_directory, 'full_crop_hemisphere.py' )
input_file = os.path.join( new_directory, 'isocortex_mask_10.nrrd' )
output_file = os.path.join( new_directory, 'isocortex_mask_crop.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 0:
	print cmd
	subprocess.call(cmd)

script_path = os.path.join( script_directory, 'full_crop_hemisphere.py' )
input_file = os.path.join( new_directory, 'laplacian_10.nrrd' )
output_file = os.path.join( new_directory, 'laplacian_crop.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if update_laplacian:
	print cmd
	subprocess.call(cmd)
	
update_streamlines = 0

# script run_streamlines.py - generate forest files
script_path = os.path.join('run_streamlines.py' )
cmd = ['python', script_path]
if update_streamlines:
	print cmd
	subprocess.call(cmd)

# script create_surface_path_index.py - generate surface_paths_10.h5
script_path = os.path.join('create_surface_path_index.py' )
cmd = ['python', script_path]
if update_streamlines:
	print cmd
	subprocess.call(cmd)
	
###
### prep for making views
###	

# create map with top surface voxels only
script_path = os.path.join( script_directory, 'remap_value.py')
input_file = os.path.join( new_directory, 'isocortex_boundary_asp.nrrd' )
output_file = os.path.join( new_directory, 'isocortex_top_surface_asp.nrrd' )
cmd = ['python', script_path, input_file, output_file, '3', '0' ]
if 0:
	print cmd
	subprocess.call(cmd)
	
script_path = os.path.join( script_directory, 'remap_value.py')
input_file = os.path.join( new_directory, 'isocortex_top_surface_asp.nrrd' )
output_file = os.path.join( new_directory, 'isocortex_top_surface_asp.nrrd' )
cmd = ['python', script_path, input_file, output_file, '4', '0' ]
if 0:
	print cmd
	subprocess.call(cmd)
	
script_path = os.path.join( script_directory, 'redo_cortex', 'binary_dilate_image.py')
input_file = os.path.join( new_directory, 'isocortex_top_surface_asp.nrrd' )
output_file = os.path.join( new_directory, 'speed_map_asp.nrrd' )
cmd = ['python', script_path, input_file, output_file, '1', '1' ]
if 0:
	print cmd
	subprocess.call(cmd)

# create views top, bottom, side, medial, front, back, rotated
script_path = os.path.join( script_directory, 'scripts','create_view_lookups.py' )
cmd = ['python', script_path]
if 0:
	print cmd
	subprocess.call(cmd)

# create flatmap
script_path = os.path.join('create_new_flatmap.py' )
cmd = ['python', script_path]
if 0:
	print cmd
	subprocess.call(cmd)
	
# create flatmap lookup view
script_path = os.path.join( script_directory, 'redo_cortex','create_flatmap_lookup.py' )
cmd = ['python', script_path]
if 0:
	print cmd
	subprocess.call(cmd)

# max projection of average template
script_path = os.path.join( script_directory, 'scripts','create_max_projection_views.py' )
input_file = '/allen/programs/celltypes/production/0378/informatics/model/P56/atlasVolume/average_template_10.nrrd'
output_directory = os.path.join(working_directory, 'average_template')
cmd = ['python', script_path, input_file, output_directory]
if 0:
	print cmd
	subprocess.call(cmd)

#
# streamline metric and fill in
#
script_path = os.path.join( script_directory, 'redo_cortex','streamline_metrics.py' )
cmd = ['python', script_path]
if 0:
	print cmd
	subprocess.call(cmd)
	
script_path = os.path.join( script_directory, 'redo_cortex','fill_mean_coordinates.py' )
cmd = ['python', script_path]
if 0:
	print cmd
	subprocess.call(cmd)


script_path = os.path.join( script_directory, 'redo_cortex','closest_streamline.py' )
cmd = ['python', script_path]
if 0:
	print cmd
	subprocess.call(cmd)



	