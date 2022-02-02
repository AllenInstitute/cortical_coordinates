import os
import subprocess

#
# Directory paths
#
working_directory = '/data/ccf/2017_integration/streamlines'
cc_directory = '/allen/programs/celltypes/production/0378/informatics/model/P56/corticalCoordinates/'
script_directory = '/data/ccf/cortical_coordinates/full/code/'
bin_directory = '/data/informatics/lydian/CCF/Q22014/CorticalCoordinates/build/bin'

# create pancake lookup table z bins
script_path = os.path.join( script_directory, 'redo_cortex','discretize_laplacian.py' )
input_file = os.path.join( working_directory, '2017', 'laplacian_10.nrrd' )
output_file = os.path.join(working_directory, 'pancake', 'pancake_depth_lookup.nrrd')
cmd = ['python', script_path, input_file, output_file]
if 0:
	print cmd
	subprocess.call(cmd)

# create a pancake for average template
script_path = os.path.join( script_directory, 'redo_cortex','create_max_pancake.py' )
input_file = '/allen/programs/celltypes/production/0378/informatics/model/P56/atlasVolume/average_template_10.nrrd'
output_file = os.path.join(working_directory, 'pancake', 'average_template.nrrd')
lookup_file = os.path.join( working_directory, 'view_lookup', 'flatmap_dorsal.h5' )
cmd = ['python', script_path, input_file, output_file, lookup_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
# create a pancake for annotation
script_path = os.path.join( script_directory, 'redo_cortex','create_max_pancake.py' )
input_file = os.path.join( '/allen/aibs/ccf/2017_integration/handoff/annotation/isocortex_annotation_10.nrrd')
output_file = os.path.join(working_directory, 'pancake', 'isocortex_annotation.nrrd')
lookup_file = os.path.join( working_directory, 'view_lookup', 'flatmap_dorsal.h5' )
cmd = ['python', script_path, input_file, output_file, lookup_file ]
if 0:
	print cmd
	subprocess.call(cmd)

# create pancake lookup table straighten z bins
script_path = os.path.join( script_directory, 'redo_cortex','straighten_layer.py' )
output_file = os.path.join(working_directory, 'pancake', 'straighten_depth_lookup.nrrd')
lookup_file = os.path.join( working_directory, 'view_lookup', 'flatmap_dorsal.h5' )
cmd = ['python', script_path, output_file, lookup_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
# create a pancake for average template
script_path = os.path.join( script_directory, 'redo_cortex','create_straighten_pancake.py' )
input_file = '/allen/programs/celltypes/production/0378/informatics/model/P56/atlasVolume/average_template_10.nrrd'
output_file = os.path.join(working_directory, 'pancake', 'straighten_average_template.nrrd')
lookup_file = os.path.join( working_directory, 'view_lookup', 'flatmap_dorsal.h5' )
depth_file = os.path.join(working_directory, 'pancake', 'straighten_depth_lookup.nrrd')
cmd = ['python', script_path, input_file, output_file, lookup_file, depth_file ]
if 0:
	print cmd
	subprocess.call(cmd)

# create a pancake for area annotation
script_path = os.path.join( script_directory, 'redo_cortex','create_straighten_pancake.py' )
input_file = os.path.join( working_directory, '2017_final', 'isocortex_annotation_10.nrrd')
output_file = os.path.join(working_directory, 'pancake', 'straighten_annotation.nrrd')
lookup_file = os.path.join( working_directory, 'view_lookup', 'flatmap_dorsal.h5' )
depth_file = os.path.join(working_directory, 'pancake', 'straighten_depth_lookup.nrrd')
cmd = ['python', script_path, input_file, output_file, lookup_file, depth_file ]
if 0:
	print cmd
	subprocess.call(cmd)
	
# create a pancake for layer annotation
script_path = os.path.join( script_directory, 'redo_cortex','create_straighten_pancake.py' )
input_file = os.path.join( working_directory, '2017', 'isocortex_layers_10.nrrd')
output_file = os.path.join(working_directory, 'pancake', 'straighten_layers.nrrd')
lookup_file = os.path.join( working_directory, 'view_lookup', 'flatmap_dorsal.h5' )
depth_file = os.path.join(working_directory, 'pancake', 'straighten_depth_lookup.nrrd')
cmd = ['python', script_path, input_file, output_file, lookup_file, depth_file ]
if 0:
	print cmd
	subprocess.call(cmd)
    
# create a pancake for projection data
script_path = os.path.join( script_directory, 'redo_cortex','create_pankcakes_projection.py' )
image_series_id = '115958825'
output_directory = os.path.join( '/allen/aibs/ccf/2017_integration/connectivity/115958825 AUDd' )
cmd = ['python', script_path, image_series_id, output_directory ]
if 0:
	print cmd
	subprocess.call(cmd)

#127866392
# create a pancake for projection data
script_path = os.path.join( script_directory, 'redo_cortex','create_pankcakes_projection.py' )
image_series_id = '127866392'
output_directory = os.path.join( '/allen/aibs/ccf/2017_integration/connectivity/127866392 SSp-bfd' )
cmd = ['python', script_path, image_series_id, output_directory ]
if 0:
	print cmd
	subprocess.call(cmd)
    
script_path = os.path.join( script_directory, 'scripts','create_max_projection_views_projection.py' )
image_series_id = '127866392'
output_directory = os.path.join( '/allen/aibs/ccf/2017_integration/connectivity/127866392 SSp-bfd' )
cmd = ['python', script_path, image_series_id, output_directory ]
if 0:
	print cmd
	subprocess.call(cmd)
    
    
#536299435
# create a pancake for projection data
script_path = os.path.join( script_directory, 'redo_cortex','create_pankcakes_projection.py' )
image_series_id = '536299435'
output_directory = os.path.join( '/allen/aibs/ccf/2017_integration/connectivity/536299435 VISp' )
cmd = ['python', script_path, image_series_id, output_directory ]
if 0:
	print cmd
	subprocess.call(cmd)
    
script_path = os.path.join( script_directory, 'scripts','create_max_projection_views_projection.py' )
image_series_id = '536299435'
output_directory = os.path.join( '/allen/aibs/ccf/2017_integration/connectivity/536299435 VISp' )
cmd = ['python', script_path, image_series_id, output_directory ]
if 0:
	print cmd
	subprocess.call(cmd)
    
#296052133
# create a pancake for projection data
script_path = os.path.join( script_directory, 'redo_cortex','create_pankcakes_projection.py' )
image_series_id = '296052133'
output_directory = os.path.join( '/allen/aibs/ccf/2017_integration/connectivity/296052133 VISp' )
cmd = ['python', script_path, image_series_id, output_directory ]
if 1:
	print cmd
	subprocess.call(cmd)
    
script_path = os.path.join( script_directory, 'scripts','create_max_projection_views_projection.py' )
image_series_id = '296052133'
output_directory = os.path.join( '/allen/aibs/ccf/2017_integration/connectivity/296052133 VISp' )
cmd = ['python', script_path, image_series_id, output_directory ]
if 1:
	print cmd
	subprocess.call(cmd)
