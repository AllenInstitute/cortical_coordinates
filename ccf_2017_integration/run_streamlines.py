import os
import sys
import subprocess


output_directory = '/data/ccf/2017_integration/streamlines/2017'
binary_directory = '/data/informatics/lydian/CCF/Q22014/CorticalCoordinates/build/bin'
#model_directory = '/projects/0378/vol1/informatics/model/P56/corticalCoordinates'

#
# compute gradient
#
binary = os.path.join( binary_directory, 'NormalizedGradient')
input_file = os.path.join( output_directory, 'laplacian_crop.nrrd')
output_file = os.path.join( output_directory, 'crop_temp_gradient.nrrd')
cmd = [binary, input_file, output_file]
if 1:
	print cmd
	subprocess.call(cmd)

binary = os.path.join( binary_directory, 'BoundaryGradient')
input_file = os.path.join( output_directory, 'laplacian_crop.nrrd')
output_file = os.path.join( output_directory, 'boundary_gradient_crop.nrrd')
boundary_file = os.path.join( output_directory, 'isocortex_boundary_crop.nrrd')
mask_file = os.path.join( output_directory, 'isocortex_mask_crop.nrrd')
gradient_file = os.path.join( output_directory, 'crop_temp_gradient.nrrd')
cmd = [binary, input_file, boundary_file, mask_file, gradient_file, output_file]
if 1:
	print cmd
	subprocess.call(cmd)

#
# Generate paths
#
binary = os.path.join( binary_directory, 'GenerateForest')
mask_file = os.path.join( output_directory, 'isocortex_mask_10.nrrd')
gradient_file = os.path.join( output_directory, 'boundary_gradient_crop.nrrd')
forest_directory = os.path.join( output_directory, 'forest' )
forest_index_file = os.path.join( output_directory, 'forest_index.nrrd')
cmd = [binary, gradient_file, mask_file, forest_directory, forest_index_file]

dir_list = [forest_directory]
for d in dir_list :
	if not os.path.exists(d):
		os.makedirs(d)
		
if 1:
	print cmd
	subprocess.call(cmd)

#
# max projection using path structure
#
binary = os.path.join( binary_directory, 'MaxProjectionForest')
input_file = '/projects/0378/vol1/informatics/model/P56/atlasVolume/average_template_10.nrrd'
output_file = os.path.join( output_directory, 'template_max_projection.nrrd' )
cmd = [binary, input_file, forest_directory, forest_index_file, output_file]
if 1:
	print cmd
	subprocess.call(cmd)








