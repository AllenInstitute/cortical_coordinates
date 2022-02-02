import os
import SimpleITK as sitk
import nrrd
import numpy as np
import h5py

output_directory = '/data/ccf/2017_integration/streamlines/2017'
#model_directory = '/projects/0378/vol1/informatics/model/P56'

forest_directory = os.path.join( output_directory, 'forest' )
reference_file = os.path.join( output_directory, 'isocortex_boundary_10.nrrd' )

#
#  open reference file
#
reference, meta = nrrd.read( reference_file )
stride =  [1, meta['sizes'][0], meta['sizes'][0] * meta['sizes'][1]]

def index2coord( ind ) :
    bind = ind - 1
    zind = (bind / stride[2])
    bind -= (zind * stride[2])
    yind = (bind / stride[1])
    bind -= (yind * stride[1])
    xind = bind
    return xind, yind, zind

#
# find pia surface voxel
#
sid = np.where( reference == 1 )  # 3D indices to surface voxels
fid = np.ravel_multi_index( sid, reference.shape ) # flat indices to surface voxels
nvoxels = len(fid) * 2

lut = np.zeros( reference.shape, np.int32 )
lut.fill(-1)

#
# create paths matrix in memory
#
paths = np.zeros( (nvoxels,200), np.uint32 )

#
# loop through all the forest files 
# 
rcount = 0

flist = os.listdir( forest_directory )
#ss = 517
#flist = flist[ss:ss+1]

for ff in flist:

    path_file = os.path.join( forest_directory, ff )
    pimg = sitk.ReadImage( path_file )
    parr = sitk.GetArrayFromImage( pimg )
    print (ff,parr.shape)

    # convert from "ad hoc" index to flat index
    ind = np.where(parr > 0)
    pp = map( index2coord, parr[ind] )
    pp = zip(*pp)
    #print (pp[0][0],pp[1][0],pp[2][0])
    ri = np.ravel_multi_index(pp,reference.shape)
    parr[ind] = ri

    # keep only voxels on the surface
    on_surface = np.in1d( parr[:,0], fid )
    parr = parr[on_surface,:]

    # add left hemisphere paths
    paths[rcount:rcount+parr.shape[0],:parr.shape[1]] = parr
    lut.flat[parr[:,0]] = range(rcount,rcount+parr.shape[0])
    rcount += parr.shape[0]
    #print "left done"

    # add right hemisphere paths
    ind = np.where(parr > 0)
    ri = parr[ind]
    pp = np.unravel_index( ri, reference.shape )
    pp = [pp[0],pp[1],tuple(reference.shape[2] - 1 - np.asarray(pp[2]))]
    #print (pp[0][0],pp[1][0],pp[2][0])
    ri = np.ravel_multi_index(pp,reference.shape)
    parr[ind] = ri
    paths[rcount:rcount+parr.shape[0],:parr.shape[1]] = parr
    lut.flat[parr[:,0]] = range(rcount,rcount+parr.shape[0])
    rcount += parr.shape[0]
    #print "right done"
    
	
print "writing output"

# initialize output h5 file
h5_file = os.path.join( output_directory, 'surface_paths_10.h5' )
f = h5py.File( h5_file, 'w' )
f.attrs['size'] = meta['sizes']
f.attrs['origin'] = meta['space origin']
f.attrs['spacing'] = np.diag(meta['space directions'])

ds = f.create_dataset('volume lookup',data=lut,compression="gzip",compression_opts=9)
ds = f.create_dataset('paths',data=paths,compression="gzip",compression_opts=9)

f.close()
