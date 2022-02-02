import os
import subprocess

#flatmap01
#anterior_seed_x = str(279)
#anterior_seed_y = str(978)
#anterior_seed_z = str(271)
#posterior_seed_x = str(302)
#posterior_seed_y = str(305)
#posterior_seed_z = str(547)
#medial_seed_x = str(66)
#medial_seed_y = str(535)
#medial_seed_z = str(309)
#lateral_seed_x = str(554)
#lateral_seed_y = str(776)
#lateral_seed_z = str(551)
#ap_threshold = str(10299.4)
#flatmap_directory = os.path.join( '/data/ccf/2017_integration/streamlines', 'flatmap01')

#flatmap02
# anterior_seed_x = str(564)
# anterior_seed_y = str(1006)
# anterior_seed_z = str(350)
# posterior_seed_x = str(143)
# posterior_seed_y = str(304)
# posterior_seed_z = str(509)
# medial_seed_x = str(119)
# medial_seed_y = str(682)
# medial_seed_z = str(247)
# lateral_seed_x = str(461)
# lateral_seed_y = str(488)
# lateral_seed_z = str(597)
# ap_threshold = str(11817.5)
# ml_threshold = str(9955.08)
# flatmap_directory = os.path.join( '/data/ccf/2017_integration/streamlines', 'flatmap02')

# flatmap03
anterior_seed_x = str(260)
anterior_seed_y = str(1035)
anterior_seed_z = str(460)
posterior_seed_x = str(278)
posterior_seed_y = str(479)
posterior_seed_z = str(731)

lateral_seed_x = str(566)
lateral_seed_y = str(843)
lateral_seed_z = str(668)
medial_seed_x = str(71)
medial_seed_y = str(662)
medial_seed_z = str(433)

flatmap_directory = os.path.join( '/data/ccf/2017_integration/streamlines', 'flatmap03')

# flatmap04
anterior_seed_x = str(547)
anterior_seed_y = str(1127)
anterior_seed_z = str(527)
posterior_seed_x = str(562)
posterior_seed_y = str(1036)
posterior_seed_z = str(356)

lateral_seed_x = str(480)
lateral_seed_y = str(1101)
lateral_seed_z = str(458)
medial_seed_x = str(568)
medial_seed_y = str(970)
medial_seed_z = str(529)

flatmap_directory = os.path.join( '/data/ccf/2017_integration/streamlines', 'flatmap04')



#
# Directory paths
#
new_directory = os.path.join( '/data/ccf/2017_integration/streamlines')
script_directory = '/data/ccf/cortical_coordinates/full/code/'
bin_directory = '/data/informatics/lydian/CCF/Q22014/CorticalCoordinates/build/bin'
new_directory = '/data/ccf/2017_integration/streamlines/2017'

# create anterior point seed map 
script_path = os.path.join( script_directory, 'redo_cortex', 'create_seed_map.py' )
input_file = os.path.join( new_directory, 'isocortex_top_surface_asp.nrrd' )
prefix = 'anterior'
output_file = os.path.join( flatmap_directory, '%s_seed_map.nrrd' % prefix )
cmd = ['python', script_path, input_file, anterior_seed_x, anterior_seed_y, anterior_seed_z, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)

# run fastmarching
bin_path = os.path.join( bin_directory, 'FastMarching' )
prefix = 'anterior'
seed_file = os.path.join( flatmap_directory, '%s_seed_map.nrrd' % prefix )
speed_file = os.path.join( new_directory, 'speed_map_asp.nrrd' )
stopping_value = str(15000)
output_file = os.path.join( flatmap_directory, '%s_distance.nii.gz' % prefix )
cmd = [bin_path,  '--stoppingValue', stopping_value, speed_file, seed_file, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)
	
# reformat to nrrd
script_path = os.path.join( script_directory, 'read_write_image.py' )
input_file = os.path.join( flatmap_directory, '%s_distance.nii.gz' % prefix )
output_file = os.path.join( flatmap_directory, '%s_distance.nrrd' % prefix )
cmd = ['python', script_path, input_file, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)
	
# create posterior point seed map 
script_path = os.path.join( script_directory, 'redo_cortex', 'create_seed_map.py' )
input_file = os.path.join( new_directory, 'isocortex_top_surface_asp.nrrd' )
prefix = 'posterior'
output_file = os.path.join( flatmap_directory, '%s_seed_map.nrrd' % prefix )
cmd = ['python', script_path, input_file, posterior_seed_x, posterior_seed_y, posterior_seed_z, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)

# run fastmarching
bin_path = os.path.join( bin_directory, 'FastMarching' )
prefix = 'posterior'
seed_file = os.path.join( flatmap_directory, '%s_seed_map.nrrd' % prefix )
speed_file = os.path.join( new_directory, 'speed_map_asp.nrrd' )
stopping_value = str(15000)
output_file = os.path.join( flatmap_directory, '%s_distance.nii.gz' % prefix )
cmd = [bin_path,  '--stoppingValue', stopping_value, speed_file, seed_file, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)
	
# reformat to nrrd
script_path = os.path.join( script_directory, 'read_write_image.py' )
input_file = os.path.join( flatmap_directory, '%s_distance.nii.gz' % prefix )
output_file = os.path.join( flatmap_directory, '%s_distance.nrrd' % prefix )
cmd = ['python', script_path, input_file, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)
	
	
# # get distance value at anterior/posterior seeds
# script_path = os.path.join( script_directory, 'redo_cortex', 'create_seed_map.py' )
# prefix = 'anterior'
# input_file = os.path.join( flatmap_directory, '%s_distance.nrrd' % prefix )
# output_file = os.path.join( flatmap_directory, 'temp.nrrd' )
# cmd = ['python', script_path, input_file, posterior_seed_x, posterior_seed_y, posterior_seed_z, output_file ]
# if 0:
	# print cmd
	# subprocess.call(cmd)
	
# script_path = os.path.join( script_directory, 'redo_cortex', 'create_seed_map.py' )
# prefix = 'posterior'
# input_file = os.path.join( flatmap_directory, '%s_distance.nrrd' % prefix )
# output_file = os.path.join( flatmap_directory, 'temp.nrrd' )
# cmd = ['python', script_path, input_file, anterior_seed_x, anterior_seed_y, anterior_seed_z, output_file ]
# if 0:
	# print cmd
	# subprocess.call(cmd)
	
# get distance to anterior/posterior line
script_path = os.path.join( script_directory, 'redo_cortex', 'compute_axes.py' )
prefix = 'anterior'
input1_file = os.path.join( flatmap_directory, '%s_distance.nrrd' % prefix )
seed1_file = os.path.join( flatmap_directory, '%s_seed_map.nrrd' % prefix )
prefix = 'posterior'
input2_file = os.path.join( flatmap_directory, '%s_distance.nrrd' % prefix )
seed2_file = os.path.join( flatmap_directory, '%s_seed_map.nrrd' % prefix )
mask_file = os.path.join( new_directory, 'speed_map_asp.nrrd' )
output1_file = os.path.join( flatmap_directory, 'anterior_posterior_axis.nrrd' )
output2_file = os.path.join( flatmap_directory, 'anterior_posterior_perpendicular.nrrd' )
cmd = ['python', script_path, input1_file, input2_file, seed1_file, seed2_file, mask_file, output1_file, output2_file ]
if 1:
	print cmd
	subprocess.call(cmd)	
	
# threshold to get axis voxels
script_path = os.path.join( script_directory, 'redo_cortex', 'binary_threshold_image.py' )
input_file = os.path.join( flatmap_directory, 'anterior_posterior_axis.nrrd' )
output_file = os.path.join( flatmap_directory, 'anterior_posterior_axis_mask.nrrd' )
cmd = ['python', script_path, input_file, output_file, '-10', '10', '5','0' ]
if 1:
	print cmd
	subprocess.call(cmd)

script_path = os.path.join( script_directory, 'mask_volume.py' )
input_file = os.path.join( flatmap_directory, 'anterior_posterior_axis_mask.nrrd' )
mask_file = os.path.join( new_directory, 'speed_map_asp.nrrd' )
output_file = os.path.join( flatmap_directory, 'anterior_posterior_axis_mask.nrrd' )
cmd = ['python', script_path, input_file, mask_file, output_file]
if 1:
	print cmd
	subprocess.call(cmd)
	
########################
# repeat for medial-lateral axis
########################
# create anterior point seed map 
script_path = os.path.join( script_directory, 'redo_cortex', 'create_seed_map.py' )
input_file = os.path.join( new_directory, 'isocortex_top_surface_asp.nrrd' )
prefix = 'medial'
output_file = os.path.join( flatmap_directory, '%s_seed_map.nrrd' % prefix )
cmd = ['python', script_path, input_file, medial_seed_x, medial_seed_y, medial_seed_z, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)

# run fastmarching
bin_path = os.path.join( bin_directory, 'FastMarching' )
prefix = 'medial'
seed_file = os.path.join( flatmap_directory, '%s_seed_map.nrrd' % prefix )
speed_file = os.path.join( new_directory, 'speed_map_asp.nrrd' )
stopping_value = str(15000)
output_file = os.path.join( flatmap_directory, '%s_distance.nii.gz' % prefix )
cmd = [bin_path,  '--stoppingValue', stopping_value, speed_file, seed_file, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)
	
# reformat to nrrd
script_path = os.path.join( script_directory, 'read_write_image.py' )
input_file = os.path.join( flatmap_directory, '%s_distance.nii.gz' % prefix )
output_file = os.path.join( flatmap_directory, '%s_distance.nrrd' % prefix )
cmd = ['python', script_path, input_file, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)
	
# create posterior point seed map 
script_path = os.path.join( script_directory, 'redo_cortex', 'create_seed_map.py' )
input_file = os.path.join( new_directory, 'isocortex_top_surface_asp.nrrd' )
prefix = 'lateral'
output_file = os.path.join( flatmap_directory, '%s_seed_map.nrrd' % prefix )
cmd = ['python', script_path, input_file, lateral_seed_x, lateral_seed_y, lateral_seed_z, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)

# run fastmarching
bin_path = os.path.join( bin_directory, 'FastMarching' )
prefix = 'lateral'
seed_file = os.path.join( flatmap_directory, '%s_seed_map.nrrd' % prefix )
speed_file = os.path.join( new_directory, 'speed_map_asp.nrrd' )
stopping_value = str(15000)
output_file = os.path.join( flatmap_directory, '%s_distance.nii.gz' % prefix )
cmd = [bin_path,  '--stoppingValue', stopping_value, speed_file, seed_file, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)
	
# reformat to nrrd
script_path = os.path.join( script_directory, 'read_write_image.py' )
input_file = os.path.join( flatmap_directory, '%s_distance.nii.gz' % prefix )
output_file = os.path.join( flatmap_directory, '%s_distance.nrrd' % prefix )
cmd = ['python', script_path, input_file, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)
	
	
# # get distance value at medial/lateral seeds
# script_path = os.path.join( script_directory, 'redo_cortex', 'create_seed_map.py' )
# prefix = 'medial'
# input_file = os.path.join( flatmap_directory, '%s_distance.nrrd' % prefix )
# output_file = os.path.join( flatmap_directory, 'temp.nrrd' )
# cmd = ['python', script_path, input_file, lateral_seed_x, lateral_seed_y, lateral_seed_z, output_file ]
# if 0:
	# print cmd
	# subprocess.call(cmd)
	
# script_path = os.path.join( script_directory, 'redo_cortex', 'create_seed_map.py' )
# prefix = 'lateral'
# input_file = os.path.join( flatmap_directory, '%s_distance.nrrd' % prefix )
# output_file = os.path.join( flatmap_directory, 'temp.nrrd' )
# cmd = ['python', script_path, input_file, medial_seed_x, medial_seed_y, medial_seed_z, output_file ]
# if 0:
	# print cmd
	# subprocess.call(cmd)

# get distance to medial/lateral line
script_path = os.path.join( script_directory, 'redo_cortex', 'compute_axes.py' )
prefix = 'medial'
input1_file = os.path.join( flatmap_directory, '%s_distance.nrrd' % prefix )
seed1_file = os.path.join( flatmap_directory, '%s_seed_map.nrrd' % prefix )
prefix = 'lateral'
input2_file = os.path.join( flatmap_directory, '%s_distance.nrrd' % prefix )
seed2_file = os.path.join( flatmap_directory, '%s_seed_map.nrrd' % prefix )
mask_file = os.path.join( new_directory, 'speed_map_asp.nrrd' )
output1_file = os.path.join( flatmap_directory, 'medial_lateral_axis.nrrd' )
output2_file = os.path.join( flatmap_directory, 'medial_lateral_perpendicular.nrrd' )
cmd = ['python', script_path, input1_file, input2_file, seed1_file, seed2_file ,mask_file, output1_file, output2_file ]
if 1:
	print cmd
	subprocess.call(cmd)	
	
# threshold to get axis voxels
script_path = os.path.join( script_directory, 'redo_cortex', 'binary_threshold_image.py' )
input_file = os.path.join( flatmap_directory, 'medial_lateral_axis.nrrd' )
output_file = os.path.join( flatmap_directory, 'medial_lateral_axis_mask.nrrd' )
cmd = ['python', script_path, input_file, output_file, '-10', '10', '6','0' ]
if 1:
	print cmd
	subprocess.call(cmd)

script_path = os.path.join( script_directory, 'mask_volume.py' )
input_file = os.path.join( flatmap_directory, 'medial_lateral_axis_mask.nrrd' )
mask_file = os.path.join( new_directory, 'speed_map_asp.nrrd' )
output_file = os.path.join( flatmap_directory, 'medial_lateral_axis_mask.nrrd' )
cmd = ['python', script_path, input_file, mask_file, output_file]
if 1:
	print cmd
	subprocess.call(cmd)

##### merge the two axis mask for visualization
	
for a in [('anterior_posterior',5),('medial_lateral',6)] :
#for a in [('medial_lateral',6)] :

	print a
	
	# create a blank map
	script_path = os.path.join( script_directory, 'redo_cortex', 'binary_threshold_image.py' )
	input_file = os.path.join( new_directory, 'speed_map_asp.nrrd' )
	output_file = os.path.join( flatmap_directory, '%s_merged_mask.nrrd' % a[0] )
	cmd = ['python', script_path, input_file, output_file, '0', '200', '0','0' ]
	if 1 :
		print cmd
		subprocess.call( cmd )
	
	for d in range(-6600,6600,200):
		print d
		script_path = os.path.join( script_directory, 'redo_cortex', 'binary_threshold_image.py' )
		input_file = os.path.join( flatmap_directory, '%s_axis.nrrd' % a[0] )
		output_file = os.path.join( flatmap_directory, 'single_axis_mask.nrrd' )
		cmd = ['python', script_path, input_file, output_file, str(d-10), str(d+10), str(a[1]),'0' ]
		if 1 :
			print cmd
			subprocess.call( cmd )
			
		script_path = os.path.join( script_directory, 'mask_volume.py' )
		input_file = os.path.join( flatmap_directory, 'single_axis_mask.nrrd' )
		mask_file = os.path.join( new_directory, 'speed_map_asp.nrrd' )
		output_file = os.path.join( flatmap_directory, 'single_axis_mask.nrrd' )
		cmd = ['python', script_path, input_file, mask_file, output_file]
		if 1:
			print cmd
			subprocess.call(cmd)

		script_path = os.path.join( script_directory, 'overwrite_image.py' )
		input1_file = os.path.join( flatmap_directory, '%s_merged_mask.nrrd' % a[0] )
		input2_file = os.path.join( flatmap_directory, 'single_axis_mask.nrrd' )
		output_file = os.path.join( flatmap_directory, '%s_merged_mask.nrrd' % a[0] )
		cmd = ['python', script_path, input1_file, input2_file, output_file]
		if 1:
			print cmd
			subprocess.call(cmd)


script_path = os.path.join( script_directory, 'overwrite_image.py' )
input1_file = os.path.join( flatmap_directory, 'anterior_posterior_merged_mask.nrrd' )
input2_file = os.path.join( flatmap_directory, 'medial_lateral_merged_mask.nrrd' )
output_file = os.path.join( flatmap_directory, 'merged_axis_mask.nrrd' )
cmd = ['python', script_path, input1_file, input2_file, output_file]
if 1:
	print cmd
	subprocess.call(cmd)
			
	
# reformat axis files into full space for creating lookup files
script_path = os.path.join( script_directory, 'flip_and_shift_volume.py' )
input_file = os.path.join( flatmap_directory, 'merged_axis_mask.nrrd' )
output_file = os.path.join( flatmap_directory, 'merged_axis_mask_10.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)

script_path = os.path.join( script_directory, 'flip_and_shift_volume.py' )
input_file = os.path.join( flatmap_directory, 'anterior_posterior_axis.nrrd' )
output_file = os.path.join( flatmap_directory, 'anterior_posterior_axis_10.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)

script_path = os.path.join( script_directory, 'flip_and_shift_volume.py' )
input_file = os.path.join( flatmap_directory, 'medial_lateral_axis.nrrd' )
output_file = os.path.join( flatmap_directory, 'medial_lateral_axis_10.nrrd' )
cmd = ['python', script_path, input_file, output_file ]
if 1:
	print cmd
	subprocess.call(cmd)




