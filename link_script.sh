# Run this script to generate links to already existing hdf5 directory
# Run this script from top directory
HDF5_VOL_DIR=/home/saksham/work/cmu/hdf5/deltafs_vol/hdf5
H5PART_VOL_DIR=/home/saksham/work/cmu/hdf5/deltafs_vol/h5part/

# If false, only removes links
CREATE_LINKS=1

zero=0;

for file in `find $HDF5_VOL_DIR/hl/src/ -maxdepth 1 -name "*.h"`; 
do 
	f=$(basename $file)
	rm -f VTK/ThirdParty/hdf5/vtkhdf5/hl/src/$f

	if [[ $CREATE_LINKS -ne $zero ]]; then
		ln -s $file VTK/ThirdParty/hdf5/vtkhdf5/hl/src/$f
	fi
done

for file in `find $HDF5_VOL_DIR/hl/src/ -maxdepth 1 -name "*.c"`; 
do 
	f=$(basename $file)
	rm  -f VTK/ThirdParty/hdf5/vtkhdf5/hl/src/$f

	if [[ $CREATE_LINKS -ne $zero ]]; then
		ln -s $file VTK/ThirdParty/hdf5/vtkhdf5/hl/src/$f
	fi
done

for file in `find $HDF5_VOL_DIR/src/ -maxdepth 1 -name "*.h"`; 
do 
	f=$(basename $file)
	rm  -f VTK/ThirdParty/hdf5/vtkhdf5/src/$f

	if [[ $CREATE_LINKS -ne $zero ]]; then
		ln -s $file VTK/ThirdParty/hdf5/vtkhdf5/src/$f
    fi
done


for file in `find $HDF5_VOL_DIR/src/ -maxdepth 1 -name "*.c"`; 
do 
	f=$(basename $file)
	rm -f VTK/ThirdParty/hdf5/vtkhdf5/src/$f

	if [[ $CREATE_LINKS -ne $zero ]]; then
		ln -s $file VTK/ThirdParty/hdf5/vtkhdf5/src/$f
	fi
done


for file in `find $H5PART_VOL_DIR/src/ -maxdepth 1 -name "*.h"`; 
do 
	f=$(basename $file)
	rm  -f ParaViewCore/VTKExtensions/H5PartReader/H5Part/src/$f

	if [[ $CREATE_LINKS -ne $zero ]]; then
		ln -s $file ParaViewCore/VTKExtensions/H5PartReader/H5Part/src/$f
	fi
done


for file in `find $H5PART_VOL_DIR/src/ -maxdepth 1 -name "*.c"`; 
do 
	f=$(basename $file)
	rm -f ParaViewCore/VTKExtensions/H5PartReader/H5Part/src/$f

	if [[ $CREATE_LINKS -ne $zero ]]; then
        ln -s $file ParaViewCore/VTKExtensions/H5PartReader/H5Part/src/$f
	fi
done


