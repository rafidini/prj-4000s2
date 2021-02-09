# -*- rpm-spec -*-
Summary:        Pink Image Processing Library
Name:           pink-image
Version:        0.9.r422
Release:        1
License:        CeCILL
Source:         pink-0.9.r422-Source.tar.bz2
Group:          Productivity/Scientific/Other
Vendor:         ESIEE Engineering
Packager:       Laszlo Marak <ujoimro@gmail.com>
BuildRequires:  boost-devel ncurses-devel cmake python-devel python-imaging python-tk IPython xorg-x11-devel gcc-c++ gcc make libtiff-devel

%description
DESCRIPTION
===========

Pink is an image processing library developed at ESIEE Engineering for
researchdir and teaching purposes. It contains implementations of over
200 algorithms for image segmentation and filtering. Most of the
operators come from mathematical morphology, but it contains operators
from different fields. It is a free software licensed under the CeCILL
license.

%prep

%ifarch i586
  archdir=lib
%else
  archdir=lib64
%endif

sourcedir=pink-0.9.r422-Source
builddir=pink-0.9.r422-Build
sourcefilename=pink-0.9.r422-Source.tar.bz2

echo "RPM_BUILD_DIR is " ${RPM_BUILD_DIR}
echo "RPM_SOURCE_DIR is " ${RPM_SOURCE_DIR}
echo "RPM_BUILD_ROOT is '${RPM_BUILD_ROOT}'"

rm -rf $RPM_BUILD_DIR/${sourcedir}
rm -rf $RPM_BUILD_DIR/${builddir}

mkdir -p ${RPM_BUILD_DIR}
cd ${RPM_BUILD_DIR}
tar xf $RPM_SOURCE_DIR/${sourcefilename}

mkdir -p $RPM_BUILD_DIR/${builddir}
cd $RPM_BUILD_DIR/${builddir}

#ls /usr/${archdir}/libpy*
#ls /usr/include/python2.6
#ls /usr/${archdir}/python2.6/*

#note: HTEST5 is a hack for openSUSE 11.2
### First configuration
cmake \
    -DPYTHON_FRONT_END=ON \
    -DCMAKE_INSTALL_PREFIX=${RPM_BUILD_ROOT}/usr \
    -DPYTHON_MODULE_INSTALL_PATH=${RPM_BUILD_ROOT}/usr/${archdir}/python2.6/site-packages \
    -DPYTHON_LIBRARIES=/usr/${archdir}/libpython2.6.so \
    -DPYTHON_LIBRARY=/usr/${archdir}/libpython2.6.so \
    -DHTEST5=FOUND \
    -DHTEST6=FOUND \
    $RPM_BUILD_DIR/${sourcedir}

### We need a second configuration for the python include files
cmake \
    -DPYTHON_FRONT_END=ON \
    -DCMAKE_INSTALL_PREFIX=${RPM_BUILD_ROOT}/usr \
    -DPYTHON_MODULE_INSTALL_PATH=${RPM_BUILD_ROOT}/usr/${archdir}/python2.6/site-packages \
    -DPYTHON_LIBRARIES=/usr/${archdir}/libpython2.6.so \
    -DPYTHON_LIBRARY=/usr/${archdir}/libpython2.6.so \
    -DHTEST5=FOUND \
    -DHTEST6=FOUND \
    $RPM_BUILD_DIR/${sourcedir}

%build

%ifarch i586
  archdir=lib
%else
  archdir=lib64
%endif

sourcedir=pink-0.9.r422-Source
builddir=pink-0.9.r422-Build
sourcefilename=pink-0.9.r422-Source.tar.bz2

make -j8 -C $RPM_BUILD_DIR/${builddir}
  
%install

%ifarch i586
  archdir=lib
%else
  archdir=lib64
%endif

sourcedir=pink-0.9.r422-Source
builddir=pink-0.9.r422-Build
sourcefilename=pink-0.9.r422-Source.tar.bz2

make -j8 -C $RPM_BUILD_DIR/pink-0.9.r422-Build install

%clean


%files

%ifarch i586
%defattr(-,root,root)
/usr/lib/libpink.so
/usr/lib/python2.6/site-packages/libcpp_pink.so
/usr/lib/python2.6/site-packages/pink/python_pink.py
/usr/lib/python2.6/site-packages/pink/isize.py
/usr/lib/python2.6/site-packages/pink/render.py
/usr/lib/python2.6/site-packages/pink/long2byte.py
/usr/lib/python2.6/site-packages/pink/cpp/__init__.py
/usr/lib/python2.6/site-packages/pink/pil_photoimage.py
/usr/lib/python2.6/site-packages/pink/MICCAI/back_to_dicom.py
/usr/lib/python2.6/site-packages/pink/MICCAI/convert_dicom.py
/usr/lib/python2.6/site-packages/pink/MICCAI/generate_moments.py
/usr/lib/python2.6/site-packages/pink/MICCAI/update.py
/usr/lib/python2.6/site-packages/pink/MICCAI/create_non_isotropized.py
/usr/lib/python2.6/site-packages/pink/MICCAI/get_a_point.py
/usr/lib/python2.6/site-packages/pink/MICCAI/isotropization.py
/usr/lib/python2.6/site-packages/pink/MICCAI/move_the_rest.py
/usr/lib/python2.6/site-packages/pink/MICCAI/registration.py
/usr/lib/python2.6/site-packages/pink/MICCAI/sage2python.py
/usr/lib/python2.6/site-packages/pink/MICCAI/__init__.py
/usr/lib/python2.6/site-packages/pink/MICCAI/headers_to_4D.py
/usr/lib/python2.6/site-packages/pink/MICCAI/itk2pink.py
/usr/lib/python2.6/site-packages/pink/manipulate.py
/usr/lib/python2.6/site-packages/pink/maxflow.py
/usr/lib/python2.6/site-packages/pink/seuilmesh.py
/usr/lib/python2.6/site-packages/pink/zoom.py
/usr/lib/python2.6/site-packages/pink/graph_plot.py
/usr/lib/python2.6/site-packages/pink/drawcurve.py
/usr/lib/python2.6/site-packages/pink/imview.py
/usr/lib/python2.6/site-packages/pink/parallel_map.py
/usr/lib/python2.6/site-packages/pink/__init__.py
/usr/lib/python2.6/site-packages/pink/show.py
/usr/lib/python2.6/site-packages/pink/view3d.py
%else
%defattr(-,root,root)
/usr/lib64/libpink.so
/usr/lib64/python2.6/site-packages/libcpp_pink.so
/usr/lib64/python2.6/site-packages/pink/python_pink.py
/usr/lib64/python2.6/site-packages/pink/isize.py
/usr/lib64/python2.6/site-packages/pink/render.py
/usr/lib64/python2.6/site-packages/pink/long2byte.py
/usr/lib64/python2.6/site-packages/pink/cpp/__init__.py
/usr/lib64/python2.6/site-packages/pink/pil_photoimage.py
/usr/lib64/python2.6/site-packages/pink/MICCAI/back_to_dicom.py
/usr/lib64/python2.6/site-packages/pink/MICCAI/convert_dicom.py
/usr/lib64/python2.6/site-packages/pink/MICCAI/generate_moments.py
/usr/lib64/python2.6/site-packages/pink/MICCAI/update.py
/usr/lib64/python2.6/site-packages/pink/MICCAI/create_non_isotropized.py
/usr/lib64/python2.6/site-packages/pink/MICCAI/get_a_point.py
/usr/lib64/python2.6/site-packages/pink/MICCAI/isotropization.py
/usr/lib64/python2.6/site-packages/pink/MICCAI/move_the_rest.py
/usr/lib64/python2.6/site-packages/pink/MICCAI/registration.py
/usr/lib64/python2.6/site-packages/pink/MICCAI/sage2python.py
/usr/lib64/python2.6/site-packages/pink/MICCAI/__init__.py
/usr/lib64/python2.6/site-packages/pink/MICCAI/headers_to_4D.py
/usr/lib64/python2.6/site-packages/pink/MICCAI/itk2pink.py
/usr/lib64/python2.6/site-packages/pink/manipulate.py
/usr/lib64/python2.6/site-packages/pink/maxflow.py
/usr/lib64/python2.6/site-packages/pink/seuilmesh.py
/usr/lib64/python2.6/site-packages/pink/zoom.py
/usr/lib64/python2.6/site-packages/pink/graph_plot.py
/usr/lib64/python2.6/site-packages/pink/drawcurve.py
/usr/lib64/python2.6/site-packages/pink/imview.py
/usr/lib64/python2.6/site-packages/pink/parallel_map.py
/usr/lib64/python2.6/site-packages/pink/__init__.py
/usr/lib64/python2.6/site-packages/pink/show.py
/usr/lib64/python2.6/site-packages/pink/view3d.py
%endif


%changelog
