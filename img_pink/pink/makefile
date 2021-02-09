PINK = .
# Version LINUX
  CC = gcc
  CPP = g++

#### use next line for NO tiff interface
  CCFLAGS = -g -DUNIXIO -Wall
#### use next line for TIFF interface
#  CCFLAGS = -g -DUNIXIO -DHAVE_TIFF_LIB -Wall

  ODIR = $(PINK)/linux/obj
  BDIR = $(PINK)/linux/bin
  IDIR = $(PINK)/include
  CDIR = $(PINK)/src/com
  LDIR = $(PINK)/src/lib
  LIARDIR = $(PINK)/src/liar
  DDIR = $(PINK)/src/bdd
  TDIR = $(PINK)/src/tables

#### use next 2 lines for NO tiff interface
  LIBS = -lm
  OBJ_COMMON = $(ODIR)/mcimage.o $(ODIR)/mcchrono.o
#### use next 2 lines for TIFF interface
#  LIBS = -lm -ltiff
#  OBJ_COMMON = $(ODIR)/mcimage.o $(ODIR)/liarwrap.o $(ODIR)/mcchrono.o
# -*- makefile -*-
OBJMCFAH=$(ODIR)/mcfah.o $(ODIR)/mcfahpure.o $(ODIR)/mcfahsalembier.o

OBJ = $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/larith.o $(ODIR)/lasft.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o $(ODIR)/ldir.o $(ODIR)/ldist.o $(ODIR)/ldistgeo.o $(ODIR)/lfiltreordre.o $(ODIR)/lgettree.o $(ODIR)/llabelextrema.o $(ODIR)/llocalextrema.o $(ODIR)/llabelplateaux.o $(ODIR)/llabeltree.o $(ODIR)/lmedialaxis.o $(ODIR)/lminima.o $(ODIR)/lvoronoilabelling.o $(ODIR)/lfmm.o $(ODIR)/bimage.o $(ODIR)/bimage_utils.o $(ODIR)/lstb_io.o $(ODIR)/fmmgeodist.o $(ODIR)/lfmmdist.o $(ODIR)/lattribarea.o $(ODIR)/lattribheight.o $(ODIR)/lattribvol.o $(ODIR)/ldynamique.o $(ODIR)/ldynamique_grimaud.o $(ODIR)/lgeodesic.o $(ODIR)/llpemeyer.o $(ODIR)/lpropgeo.o $(ODIR)/lsaliency.o $(ODIR)/lsegreconsheight.o $(ODIR)/lsegment3d.o $(ODIR)/lsegmentnum.o $(ODIR)/lwshedtopo.o $(ODIR)/lborder.o $(ODIR)/lcurves.o $(ODIR)/lfiltrestopo.o $(ODIR)/lfermetrous3d.o $(ODIR)/lhthiniso.o $(ODIR)/lhtkern.o $(ODIR)/lhtkern3d.o $(ODIR)/lkern.o $(ODIR)/llambdakern.o $(ODIR)/llambdalevkern.o $(ODIR)/llambdasquel.o $(ODIR)/llpetopo.o $(ODIR)/llpetoporeg.o $(ODIR)/lnbtopo.o $(ODIR)/lnbvois.o $(ODIR)/lskel2graph.o $(ODIR)/lskelcurv.o $(ODIR)/lseltopo.o $(ODIR)/lsquel.o $(ODIR)/lsquelbin.o $(ODIR)/lskeletons.o $(ODIR)/lskelpar.o $(ODIR)/lskelpar3d.o $(ODIR)/lskel_ACK3a.o $(ODIR)/lskelpar3d_others.o $(ODIR)/lsquelval.o $(ODIR)/ltopotypes.o $(ODIR)/DirectionalSkeletonizer.o $(ODIR)/l2dkhalimsky.o $(ODIR)/l2dcollapse.o $(ODIR)/l3dkhalimsky.o $(ODIR)/l3dcollapse.o $(ODIR)/ldraw.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/lseamcarving.o $(ODIR)/lattribute.o $(ODIR)/lballincl.o $(ODIR)/lbarycentre.o $(ODIR)/lbresen.o $(ODIR)/lconvexhull.o $(ODIR)/lcrop.o $(ODIR)/libcrop.o $(ODIR)/ldetectcercles.o $(ODIR)/ldirections.o $(ODIR)/leden.o $(ODIR)/lellipsefit.o $(ODIR)/lrecalagerigide.o $(ODIR)/lrecalagerigide_translateplane.o $(ODIR)/lellipseincl.o $(ODIR)/lgeo.o $(ODIR)/lhoughcercles.o $(ODIR)/lisometry.o $(ODIR)/llenoir.o $(ODIR)/lmaxdiameter.o $(ODIR)/lmoments.o $(ODIR)/loffset.o $(ODIR)/lpoint.o $(ODIR)/lrotations.o $(ODIR)/lselrect.o $(ODIR)/lsym.o $(ODIR)/lwarp.o $(ODIR)/lvoronoi.o $(ODIR)/lzoom.o $(ODIR)/lzoomint.o $(ODIR)/lcomphisto.o $(ODIR)/legalise.o $(ODIR)/lhisto.o $(ODIR)/lhistscal.o $(ODIR)/lstretch.o $(ODIR)/lstat.o $(ODIR)/lconvol.o $(ODIR)/lconvol3.o $(ODIR)/ldiZenzo.o $(ODIR)/lderiche.o $(ODIR)/lfft.o $(ODIR)/lmeanfilter.o $(ODIR)/lgradill.o $(ODIR)/lselndg.o $(ODIR)/lseuil.o $(ODIR)/lskew.o $(ODIR)/mccbt.o $(ODIR)/mcchrono.o $(ODIR)/mccodimage.o $(ODIR)/mccolors.o $(ODIR)/mccomptree.o $(ODIR)/mcdrawps.o $(ODIR)/mcfah.o $(ODIR)/mcfahpure.o $(ODIR)/mcfahsalembier.o $(ODIR)/mcfifo.o $(ODIR)/mcfusion.o $(ODIR)/mcgeo.o $(ODIR)/mcgraphe.o $(ODIR)/mcimage.o $(ODIR)/mcindic.o $(ODIR)/mciomesh.o $(ODIR)/mckhalimsky2d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mckhalimskyNd.o $(ODIR)/mclifo.o $(ODIR)/mclin.o $(ODIR)/mcliste.o $(ODIR)/mcmesh.o $(ODIR)/mcpolygons.o $(ODIR)/mcpowell.o $(ODIR)/mcprobas.o $(ODIR)/mcrbt.o $(ODIR)/mcrbtp.o $(ODIR)/mcrlifo.o $(ODIR)/mcsegment.o $(ODIR)/mcset.o $(ODIR)/mcskel3d.o $(ODIR)/mcskel2d.o $(ODIR)/mcskel2d3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mcsplines.o $(ODIR)/mcsort.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo3d_table.o $(ODIR)/mctree.o $(ODIR)/mcunionfind.o $(ODIR)/avscrop.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/llpemeyer4d.o $(ODIR)/llpeGA.o $(ODIR)/jcgraphes.o $(ODIR)/lhierarchie.o $(ODIR)/lMSF.o $(ODIR)/jccodimage.o $(ODIR)/jccomptree.o $(ODIR)/lppm2GA.o $(ODIR)/lga2khalimsky.o $(ODIR)/jcimage.o $(ODIR)/ssexport.o $(ODIR)/liarwrap.o 

OBJ_OLD = $(ODIR)/mcliste.o $(ODIR)/mcset.o $(ODIR)/mckhalimsky2d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mckhalimskyNd.o $(ODIR)/mcchrono.o $(ODIR)/mctopo3d_table.o $(ODIR)/mcdrawps.o $(ODIR)/mcsegment.o

ARITH=\
$(BDIR)/add \
$(BDIR)/addconst \
$(BDIR)/area \
$(BDIR)/argmax \
$(BDIR)/argmin \
$(BDIR)/average \
$(BDIR)/complex_modulus \
$(BDIR)/complex_real \
$(BDIR)/complex_imaginary \
$(BDIR)/correctbias \
$(BDIR)/diffimages \
$(BDIR)/div \
$(BDIR)/dup \
$(BDIR)/equal \
$(BDIR)/exp \
$(BDIR)/genfield \
$(BDIR)/genimage \
$(BDIR)/genkernel \
$(BDIR)/gammacor \
$(BDIR)/inf \
$(BDIR)/inverse \
$(BDIR)/isnull \
$(BDIR)/log \
$(BDIR)/mask \
$(BDIR)/max \
$(BDIR)/max1 \
$(BDIR)/min \
$(BDIR)/min1 \
$(BDIR)/moyenne \
$(BDIR)/mult \
$(BDIR)/normalize \
$(BDIR)/otsu \
$(BDIR)/seuilOtsu \
$(BDIR)/pow \
$(BDIR)/scale \
$(BDIR)/selndg \
$(BDIR)/seuil \
$(BDIR)/seuil2 \
$(BDIR)/seuilauto \
$(BDIR)/sub \
$(BDIR)/sup \
$(BDIR)/threshold \
$(BDIR)/volume \
$(BDIR)/xor

CONVERT=\
$(BDIR)/2elisp \
$(BDIR)/2latex \
$(BDIR)/ac2pink \
$(BDIR)/any2complex \
$(BDIR)/bmp2ppm \
$(BDIR)/bmp2pgm \
$(BDIR)/byte2float \
$(BDIR)/byte2long \
$(BDIR)/car2pol \
$(BDIR)/catgif \
$(BDIR)/catpgm \
$(BDIR)/changename \
$(BDIR)/concat3d \
$(BDIR)/double2byte \
$(BDIR)/explode \
$(BDIR)/explodecomp \
$(BDIR)/extrude \
$(BDIR)/float2byte \
$(BDIR)/float2long \
$(BDIR)/graph2pgm \
$(BDIR)/graph2ps \
$(BDIR)/hls2rgb \
$(BDIR)/list2pgm \
$(BDIR)/long2byte \
$(BDIR)/long2float \
$(BDIR)/mhd2pgm \
$(BDIR)/nifti2pgm \
$(BDIR)/ombre \
$(BDIR)/pgm2bmp \
$(BDIR)/pgm2closedcurve \
$(BDIR)/pgm2curve \
$(BDIR)/pgm2graph \
$(BDIR)/pgm2list \
$(BDIR)/pgm2pov \
$(BDIR)/pgm2ppm \
$(BDIR)/pgm2ps \
$(BDIR)/pgm2raw \
$(BDIR)/pgm2slc \
$(BDIR)/pgm2vtk \
$(BDIR)/pgmascmode \
$(BDIR)/pgmrawmode \
$(BDIR)/pol2car \
$(BDIR)/ppm2bmp \
$(BDIR)/ppm2pgm \
$(BDIR)/ppmascmode \
$(BDIR)/raw2pgm \
$(BDIR)/rawthreshold \
$(BDIR)/readgif \
$(BDIR)/reformat \
$(BDIR)/rgb2hls \
$(BDIR)/rgb2ppm \
$(BDIR)/sceneconvert \
$(BDIR)/setorigin \
$(BDIR)/setvoxdim \
$(BDIR)/short2float \
$(BDIR)/short2long \
$(BDIR)/surimp \
$(BDIR)/yuv2rgb

MORPHO=\
$(BDIR)/asf \
$(BDIR)/asfbin \
$(BDIR)/asflin \
$(BDIR)/asfr \
$(BDIR)/bisector \
$(BDIR)/bisector_talbot \
$(BDIR)/closeball \
$(BDIR)/closing \
$(BDIR)/dilat \
$(BDIR)/dilatball \
$(BDIR)/dilatballnum \
$(BDIR)/dilatbin3d \
$(BDIR)/dilatbin \
$(BDIR)/dilatfast \
$(BDIR)/dilation \
$(BDIR)/dilatnum \
$(BDIR)/dir \
$(BDIR)/dist \
$(BDIR)/distc \
$(BDIR)/distgeo \
$(BDIR)/distsets \
$(BDIR)/dynamique \
$(BDIR)/dynamiquefilter \
$(BDIR)/eros \
$(BDIR)/erosball \
$(BDIR)/erosballnum \
$(BDIR)/erosbin \
$(BDIR)/erosbin3d \
$(BDIR)/erosion \
$(BDIR)/erosnum \
$(BDIR)/filtreordre \
$(BDIR)/filtrerang \
$(BDIR)/granulometry \
$(BDIR)/hitormiss \
$(BDIR)/integermedialaxis \
$(BDIR)/lambdamedialaxis \
$(BDIR)/lambdamedialaxisauto \
$(BDIR)/lambdamedialaxis2 \
$(BDIR)/lambdamedialaxis2auto \
$(BDIR)/lambdapmedialaxis \
$(BDIR)/lantuejoul \
$(BDIR)/lintophat \
$(BDIR)/medianfilter \
$(BDIR)/medialaxis \
$(BDIR)/openball \
$(BDIR)/opening \
$(BDIR)/openingfunction \
$(BDIR)/radialopening \
$(BDIR)/radialrankmaxopening \
$(BDIR)/rankfilter \
$(BDIR)/rankmaxopening \
$(BDIR)/redt \
$(BDIR)/scaleaxis \
$(BDIR)/scalefilteredmedialaxis \
$(BDIR)/sfema \
$(BDIR)/ultimateerosion \
$(BDIR)/voronoilabelling \
$(BDIR)/fmm

CONNECT=\
$(BDIR)/amont \
$(BDIR)/areaclosing \
$(BDIR)/areaopening \
$(BDIR)/areaselnb \
$(BDIR)/deletecomp \
$(BDIR)/geodilat \
$(BDIR)/geoeros \
$(BDIR)/heightmaxima \
$(BDIR)/heightminima \
$(BDIR)/heightselnb \
$(BDIR)/label \
$(BDIR)/labeldil \
$(BDIR)/labelfgd \
$(BDIR)/nbcomp \
$(BDIR)/propgeo \
$(BDIR)/saliency \
$(BDIR)/segmentarea \
$(BDIR)/segmentheight \
$(BDIR)/segmentnum \
$(BDIR)/segmentvol \
$(BDIR)/selectcomp \
$(BDIR)/shortestpath \
$(BDIR)/volmaxima \
$(BDIR)/volselnb \
$(BDIR)/watershed \
$(BDIR)/watershedMeyer \
$(BDIR)/watershedMeyer2 \
$(BDIR)/watershedMeyer2lab \
$(BDIR)/watershedMeyer3 \
$(BDIR)/watershedthin \
$(BDIR)/watershedwithoutline \
$(BDIR)/watershedwithoutlinelab \
$(BDIR)/wshedtopo

GA=\
$(BDIR)/pgm2GA \
$(BDIR)/ppm2GA \
$(BDIR)/pgm2GA4d \
$(BDIR)/GA2pgm \
$(BDIR)/saliencyGA \
$(BDIR)/watershedMeyer4D \
$(BDIR)/wshedval \
$(BDIR)/wshedkhalimsky \
$(BDIR)/MSF \
$(BDIR)/MSF4d \
$(BDIR)/GA2khalimsky \
$(BDIR)/GA2tikz \
$(BDIR)/GAwatershed \
$(BDIR)/GAwatershedDouble

TOPO=\
$(BDIR)/alpha \
$(BDIR)/asft \
$(BDIR)/asftmed \
$(BDIR)/asftndg \
$(BDIR)/border \
$(BDIR)/crestheight \
$(BDIR)/crestrestoration \
$(BDIR)/crestsegment \
$(BDIR)/crestvol \
$(BDIR)/deletepeaks \
$(BDIR)/deletewells \
$(BDIR)/grayskel \
$(BDIR)/hcutting \
$(BDIR)/hdilatball \
$(BDIR)/herosball \
$(BDIR)/hfilling \
$(BDIR)/hthick \
$(BDIR)/hthickbin \
$(BDIR)/hthin \
$(BDIR)/hthinbin \
$(BDIR)/hthiniso \
$(BDIR)/hthinpar \
$(BDIR)/htkern \
$(BDIR)/htkern3dbin \
$(BDIR)/htkernu \
$(BDIR)/holeclosing \
$(BDIR)/kern \
$(BDIR)/labeltree \
$(BDIR)/lambdalthin \
$(BDIR)/lambdathin \
$(BDIR)/lambdaskel \
$(BDIR)/localextrema \
$(BDIR)/lpetopo \
$(BDIR)/lpetoporeg \
$(BDIR)/lthin \
$(BDIR)/lthick \
$(BDIR)/lvkern \
$(BDIR)/lvkernu \
$(BDIR)/maxima \
$(BDIR)/minima \
$(BDIR)/minimalsimplepair \
$(BDIR)/nbtopo \
$(BDIR)/nbvois \
$(BDIR)/pgm2skel \
$(BDIR)/pgm2skel2 \
$(BDIR)/pgm2vskel \
$(BDIR)/ptcurve \
$(BDIR)/ptend \
$(BDIR)/ptextensible \
$(BDIR)/pthseparatinggray \
$(BDIR)/ptinterior \
$(BDIR)/ptjunction \
$(BDIR)/ptisolated \
$(BDIR)/ptmultiple \
$(BDIR)/ptselectgray \
$(BDIR)/ptseparating \
$(BDIR)/ptseparatinggray \
$(BDIR)/ptsimple \
$(BDIR)/ptthickend \
$(BDIR)/ptthickisthmus \
$(BDIR)/ptWsimple \
$(BDIR)/seltopo \
$(BDIR)/simplepair \
$(BDIR)/squel \
$(BDIR)/skel2graph \
$(BDIR)/skel2pgm \
$(BDIR)/skel2pov \
$(BDIR)/skel_ACK3 \
$(BDIR)/skel_ACK3a \
$(BDIR)/skel_ACK3_pers \
$(BDIR)/skel_AK2 \
$(BDIR)/skel_ASK3p \
$(BDIR)/skel_AMK \
$(BDIR)/skel_CK2_pers \
$(BDIR)/skel_CK3a \
$(BDIR)/skel_CK3p \
$(BDIR)/skel_CK3_pers \
$(BDIR)/skel_CKG \
$(BDIR)/skel_CKG_map \
$(BDIR)/skel_CKSC2 \
$(BDIR)/skel_CKSS3 \
$(BDIR)/skel_EK3 \
$(BDIR)/skel_MK2 \
$(BDIR)/skel_MK3 \
$(BDIR)/skel_NK2 \
$(BDIR)/skel_NK2_pers \
$(BDIR)/skel_PSG \
$(BDIR)/skel_SCK3_pers \
$(BDIR)/skel_SK3 \
$(BDIR)/skel_SK3_pers \
$(BDIR)/skelend \
$(BDIR)/skelendlab \
$(BDIR)/skelcurv \
$(BDIR)/skelcurv2 \
$(BDIR)/skelcurvend \
$(BDIR)/skelcurvfilter \
$(BDIR)/skeleton \
$(BDIR)/skeletonlab \
$(BDIR)/skeleucl \
$(BDIR)/skelfindelbows \
$(BDIR)/skelpar \
$(BDIR)/skelpar3d \
$(BDIR)/skelpar3d_others \
$(BDIR)/skelpruning \
$(BDIR)/skelsmoothing \
$(BDIR)/skelsurf \
$(BDIR)/skelvertex \
$(BDIR)/squelval \
$(BDIR)/surfacerestoration \
$(BDIR)/t26pp \
$(BDIR)/t4pp \
$(BDIR)/t8pp \
$(BDIR)/taf \
$(BDIR)/tlf \
$(BDIR)/tuf \
$(BDIR)/toposhrink \
$(BDIR)/toposhrinkgray \
$(BDIR)/topotypes

ORDRES=\
$(BDIR)/2dtopoflow \
$(BDIR)/2dborder \
$(BDIR)/2dcollapse \
$(BDIR)/2dcolor \
$(BDIR)/2dflowskeleton \
$(BDIR)/2dinvariants \
$(BDIR)/2dkhalimskize \
$(BDIR)/2dlabel \
$(BDIR)/2dmakecomplex \
$(BDIR)/2dmakesimplicialcomplex \
$(BDIR)/2dpardircollapse \
$(BDIR)/2dseltopo \
$(BDIR)/2dskelck \
$(BDIR)/2dthin \
$(BDIR)/3dalpha \
$(BDIR)/3dbeta \
$(BDIR)/3dborder \
$(BDIR)/3dcollapse \
$(BDIR)/3dpardircollapse \
$(BDIR)/3dsurfacecollapse \
$(BDIR)/3dcolor \
$(BDIR)/3dflowskeleton \
$(BDIR)/3dinvariants \
$(BDIR)/3disthmus \
$(BDIR)/3dkhalimskize \
$(BDIR)/3dlabel \
$(BDIR)/3dmakecomplex \
$(BDIR)/3dmakepolygons \
$(BDIR)/3dplane \
$(BDIR)/3drecons \
$(BDIR)/3dseltopo \
$(BDIR)/3dskelck \
$(BDIR)/3dskelsurf \
$(BDIR)/3dthin

DRAW=\
$(BDIR)/animscenes \
$(BDIR)/drawball \
$(BDIR)/drawcurve \
$(BDIR)/drawcylinder \
$(BDIR)/drawellipse \
$(BDIR)/drawfield \
$(BDIR)/drawfieldlists \
$(BDIR)/drawgraph \
$(BDIR)/drawline \
$(BDIR)/drawlines \
$(BDIR)/drawquadrangle \
$(BDIR)/drawrect \
$(BDIR)/drawscene \
$(BDIR)/drawspline \
$(BDIR)/drawsplineorient \
$(BDIR)/drawsplinesorient \
$(BDIR)/drawsplinesorient2 \
$(BDIR)/drawsplines \
$(BDIR)/drawtorus \
$(BDIR)/drawtriangle \
$(BDIR)/drawtriangulation \
$(BDIR)/genball \
$(BDIR)/line \
$(BDIR)/curve2spline \
$(BDIR)/points2spline \
$(BDIR)/samplespline \
$(BDIR)/worms

GEO=\
$(BDIR)/affine \
$(BDIR)/attribute \
$(BDIR)/autocrop \
$(BDIR)/barycentre \
$(BDIR)/barycentrelab \
$(BDIR)/boxmin \
$(BDIR)/cadre \
$(BDIR)/convexhull \
$(BDIR)/crop \
$(BDIR)/cropondisk \
$(BDIR)/curvatures \
$(BDIR)/curve2segments \
$(BDIR)/curvelength \
$(BDIR)/curvenormalplanes \
$(BDIR)/curvetangents \
$(BDIR)/curvesections \
$(BDIR)/deframe \
$(BDIR)/delaunay \
$(BDIR)/delaunaymask \
$(BDIR)/detectcercles \
$(BDIR)/directions \
$(BDIR)/eden \
$(BDIR)/ellipsefind \
$(BDIR)/ellipsefit \
$(BDIR)/ellipseincl \
$(BDIR)/ellipseparams \
$(BDIR)/ellipticite \
$(BDIR)/encadre \
$(BDIR)/enframe \
$(BDIR)/enframe3d \
$(BDIR)/expandframe \
$(BDIR)/extractline \
$(BDIR)/extractplane \
$(BDIR)/fitcircle \
$(BDIR)/frame \
$(BDIR)/gencurv \
$(BDIR)/genplane \
$(BDIR)/houghcercles \
$(BDIR)/identifyline \
$(BDIR)/identifyparabola2 \
$(BDIR)/identifyparabola3 \
$(BDIR)/identifyplane \
$(BDIR)/insert \
$(BDIR)/isometry \
$(BDIR)/lengthspline \
$(BDIR)/lengthsplines \
$(BDIR)/matchellipse \
$(BDIR)/matchrect \
$(BDIR)/maxdiameter \
$(BDIR)/moments \
$(BDIR)/offset \
$(BDIR)/pca \
$(BDIR)/planarity \
$(BDIR)/point \
$(BDIR)/proj \
$(BDIR)/proj3d \
$(BDIR)/projsphere \
$(BDIR)/quasishear \
$(BDIR)/recalagerigide \
$(BDIR)/recalagerigide_num \
$(BDIR)/recalagerigide_translateplane \
$(BDIR)/rotate \
$(BDIR)/rotate3dbin \
$(BDIR)/rotatebin \
$(BDIR)/rotse \
$(BDIR)/section \
$(BDIR)/selrect \
$(BDIR)/shrinkondisk \
$(BDIR)/surrect \
$(BDIR)/sym \
$(BDIR)/symse \
$(BDIR)/traceellipses \
$(BDIR)/warp \
$(BDIR)/zoom \
$(BDIR)/zoomint \
$(BDIR)/zoomrgb \

HISTO=\
$(BDIR)/comphisto \
$(BDIR)/countvalues \
$(BDIR)/egalise \
$(BDIR)/histo \
$(BDIR)/histo2 \
$(BDIR)/histoazimuth \
$(BDIR)/histoazimuth2 \
$(BDIR)/histodist \
$(BDIR)/histoelevation \
$(BDIR)/histoelevation2 \
$(BDIR)/histolisse \
$(BDIR)/histopgm \
$(BDIR)/histscal \
$(BDIR)/histosieve \
$(BDIR)/histstretch \
$(BDIR)/printhisto \
$(BDIR)/printstats \
$(BDIR)/relabel \
$(BDIR)/seuilhisto \
$(BDIR)/stretch

SIGNAL=\
$(BDIR)/average1 \
$(BDIR)/ccv \
$(BDIR)/convol \
$(BDIR)/convol3 \
$(BDIR)/diZenzo \
$(BDIR)/deriche \
$(BDIR)/deriche3d \
$(BDIR)/directionalfilter \
$(BDIR)/ecarttype \
$(BDIR)/entropy \
$(BDIR)/fft \
$(BDIR)/gradientcd \
$(BDIR)/gaussianfilter \
$(BDIR)/laplacian \
$(BDIR)/longestplateau \
$(BDIR)/meanfilter \
$(BDIR)/shencastan \
$(BDIR)/standarddeviation1 \
$(BDIR)/variance1

MESH3D=\
$(BDIR)/mcube\
$(BDIR)/surfels\
$(BDIR)/meshaddnoise\
$(BDIR)/meshconvert\
$(BDIR)/meshflatten\
$(BDIR)/meshregul\
$(BDIR)/mcm2ac\
$(BDIR)/mcm2vtk\
$(BDIR)/mcmbuild

DIVERS=\
$(BDIR)/bruite \
$(BDIR)/interpolate \
$(BDIR)/colorize \
$(BDIR)/genbicol \
$(BDIR)/genbini \
$(BDIR)/genlut \
$(BDIR)/gradill \
$(BDIR)/grid \
$(BDIR)/kanungonoise \
$(BDIR)/randimage \
$(BDIR)/randpoints \
$(BDIR)/randrgb \
$(BDIR)/seamcarving \
$(BDIR)/shake \
$(BDIR)/showheader \
$(BDIR)/showpoint \
$(BDIR)/skew

EXPERIMENTAL=\
$(BDIR)/3dsphere \
$(BDIR)/contours \
$(BDIR)/dynrecons \
$(BDIR)/jones \
$(BDIR)/lenoir \
$(BDIR)/levialdi \
$(BDIR)/reconsplateaux \
$(BDIR)/regul \
$(BDIR)/remspnoise \
$(BDIR)/segmentlignes \
$(BDIR)/split \
$(BDIR)/souzabanon

ANDRE=\
$(ODIR)/avsimage.o \
$(ODIR)/avscrop.o \
$(ODIR)/lhma.o \
$(ODIR)/llut.o \
$(BDIR)/hma

TABLES=\
$(PINK)/src/tables/SimpleTable \
$(PINK)/src/tables/mhlut

TRASH=\
$(BDIR)/erosfast \
$(BDIR)/erosfast3d \
$(BDIR)/waterfall

all:	$(ODIR)/libpink.a $(OBJ) $(ARITH) $(CONVERT) $(MORPHO) $(CONNECT) $(GA) $(TOPO) $(ORDRES) $(DRAW) $(GEO) $(HISTO) $(SIGNAL) $(DIVERS) $(MESH3D) doc

doc:	$(PINK)/pink.dox
	doxygen $(PINK)/pink.dox
ifeq ($(HOME),/user/coupriem)
	cp $(PINK)/doc/html/* $(HOME)/public_html/Pink/doc/html
endif	

clean:	
	rm -f $(BDIR)/*
	rm -f $(ODIR)/*
	rm -f $(CDIR)/*~
	rm -f $(LDIR)/*~
	rm -f $(IDIR)/*~
	rm -f $(PINK)/*~
	rm -f $(PINK)/*.o
	rm -f $(PINK)/doc/*~
	rm -f $(PINK)/scripts/*~
	rm -f $(PINK)/tcl/*~

cleanall:
	make clean
	rm -f $(PINK)/makefile
	rm -f $(PINK)/doc/latex/*
	rm -f $(PINK)/doc/html/*

tgz:
	cp -R $(PINK) /tmp/Pinktmp
	cd /tmp/Pinktmp; make clean
	rm -f -r /tmp/Pinktmp/.svn
	rm -f -r /tmp/Pinktmp/*/.svn
	rm -f -r /tmp/Pinktmp/*/*/.svn
	rm -f -r /tmp/Pinktmp/*/*/*/.svn
	cd /tmp/Pinktmp/Bdd ; make clean
	cd /tmp; tar zcvf pink.tgz Pinktmp; mv pink.tgz $(HOME)/pink.tgz 
	rm -f -r /tmp/Pinktmp

# ===============================================================
# LIBRAIRIE
# ===============================================================
$(ODIR)/libpink.a:	\
$(ODIR)/bdd1.alphacube.o \
$(ODIR)/bdd2.alpha.o \
$(ODIR)/bdd2.beta.o \
$(ODIR)/bdd3.o \
$(ODIR)/larith.o \
$(ODIR)/lasft.o \
$(ODIR)/ldilateros.o \
$(ODIR)/ldilateros3d.o \
$(ODIR)/ldir.o \
$(ODIR)/ldist.o \
$(ODIR)/ldistgeo.o \
$(ODIR)/lfiltreordre.o \
$(ODIR)/lgettree.o \
$(ODIR)/llabelextrema.o \
$(ODIR)/llocalextrema.o \
$(ODIR)/llabelplateaux.o \
$(ODIR)/llabeltree.o \
$(ODIR)/lmedialaxis.o \
$(ODIR)/lminima.o \
$(ODIR)/lvoronoilabelling.o \
$(ODIR)/lfmm.o \
$(ODIR)/bimage.o \
$(ODIR)/bimage_utils.o \
$(ODIR)/lstb_io.o \
$(ODIR)/fmmgeodist.o \
$(ODIR)/lfmmdist.o \
$(ODIR)/lattribarea.o \
$(ODIR)/lattribheight.o \
$(ODIR)/lattribvol.o \
$(ODIR)/ldynamique.o \
$(ODIR)/ldynamique_grimaud.o \
$(ODIR)/lgeodesic.o \
$(ODIR)/llpemeyer.o \
$(ODIR)/lpropgeo.o \
$(ODIR)/lsaliency.o \
$(ODIR)/lsegreconsheight.o \
$(ODIR)/lsegment3d.o \
$(ODIR)/lsegmentnum.o \
$(ODIR)/lwshedtopo.o \
$(ODIR)/lborder.o \
$(ODIR)/lcurves.o \
$(ODIR)/lfiltrestopo.o \
$(ODIR)/lfermetrous3d.o \
$(ODIR)/lhthiniso.o \
$(ODIR)/lhtkern.o \
$(ODIR)/lhtkern3d.o \
$(ODIR)/lkern.o \
$(ODIR)/llambdakern.o \
$(ODIR)/llambdalevkern.o \
$(ODIR)/llambdasquel.o \
$(ODIR)/llpetopo.o \
$(ODIR)/llpetoporeg.o \
$(ODIR)/lnbtopo.o \
$(ODIR)/lnbvois.o \
$(ODIR)/lskel2graph.o \
$(ODIR)/lskelcurv.o \
$(ODIR)/lseltopo.o \
$(ODIR)/lsquel.o \
$(ODIR)/lsquelbin.o \
$(ODIR)/lskel_ACK3a.o \
$(ODIR)/lskeletons.o \
$(ODIR)/lskelpar.o \
$(ODIR)/lskelpar3d.o \
$(ODIR)/lskelpar3d_others.o \
$(ODIR)/lsquelval.o \
$(ODIR)/ltopotypes.o \
$(ODIR)/DirectionalSkeletonizer.o \
$(ODIR)/l2dkhalimsky.o \
$(ODIR)/l2dcollapse.o \
$(ODIR)/l3dkhalimsky.o \
$(ODIR)/l3dcollapse.o \
$(ODIR)/ldraw.o \
$(ODIR)/lbdigitalline.o \
$(ODIR)/ltangents.o \
$(ODIR)/lseamcarving.o \
$(ODIR)/lattribute.o \
$(ODIR)/lballincl.o \
$(ODIR)/lbarycentre.o \
$(ODIR)/lbresen.o \
$(ODIR)/lconvexhull.o \
$(ODIR)/lcrop.o \
$(ODIR)/libcrop.o \
$(ODIR)/ldetectcercles.o \
$(ODIR)/ldirections.o \
$(ODIR)/leden.o \
$(ODIR)/lellipsefit.o \
$(ODIR)/lrecalagerigide.o \
$(ODIR)/lrecalagerigide_translateplane.o \
$(ODIR)/lellipseincl.o \
$(ODIR)/lgeo.o \
$(ODIR)/lhoughcercles.o \
$(ODIR)/lisometry.o \
$(ODIR)/llenoir.o \
$(ODIR)/lmaxdiameter.o \
$(ODIR)/lmoments.o \
$(ODIR)/loffset.o \
$(ODIR)/lpoint.o \
$(ODIR)/lrotations.o \
$(ODIR)/lselrect.o \
$(ODIR)/lsym.o \
$(ODIR)/lwarp.o \
$(ODIR)/lvoronoi.o \
$(ODIR)/lzoom.o \
$(ODIR)/lzoomint.o \
$(ODIR)/lcomphisto.o \
$(ODIR)/legalise.o \
$(ODIR)/lhisto.o \
$(ODIR)/lhistscal.o \
$(ODIR)/lstretch.o \
$(ODIR)/lstat.o \
$(ODIR)/lconvol.o \
$(ODIR)/lconvol3.o \
$(ODIR)/ldiZenzo.o \
$(ODIR)/lderiche.o \
$(ODIR)/lfft.o \
$(ODIR)/lmeanfilter.o \
$(ODIR)/lgradill.o \
$(ODIR)/lselndg.o \
$(ODIR)/lseuil.o \
$(ODIR)/lskew.o \
$(ODIR)/mccbt.o \
$(ODIR)/mcchrono.o \
$(ODIR)/mccodimage.o \
$(ODIR)/mccolors.o \
$(ODIR)/mccomptree.o \
$(ODIR)/mcdrawps.o \
$(ODIR)/mcfah.o \
$(ODIR)/mcfahpure.o \
$(ODIR)/mcfahsalembier.o \
$(ODIR)/mcfifo.o \
$(ODIR)/mcfusion.o \
$(ODIR)/mcgeo.o \
$(ODIR)/mcgraphe.o \
$(ODIR)/mcimage.o \
$(ODIR)/mcindic.o \
$(ODIR)/mciomesh.o \
$(ODIR)/mckhalimsky2d.o \
$(ODIR)/mckhalimsky3d.o \
$(ODIR)/mckhalimskyNd.o \
$(ODIR)/mclifo.o \
$(ODIR)/mclin.o \
$(ODIR)/mcliste.o \
$(ODIR)/mcmesh.o \
$(ODIR)/mcpolygons.o \
$(ODIR)/mcpowell.o \
$(ODIR)/mcprobas.o \
$(ODIR)/mcrbt.o \
$(ODIR)/mcrbtp.o \
$(ODIR)/mcrlifo.o \
$(ODIR)/mcsegment.o \
$(ODIR)/mcset.o \
$(ODIR)/mcskel3d.o \
$(ODIR)/mcskel2d.o \
$(ODIR)/mcskel2d3d.o \
$(ODIR)/mcskelcurv.o \
$(ODIR)/mcsplines.o \
$(ODIR)/mcsort.o \
$(ODIR)/mctopo.o \
$(ODIR)/mctopo3d.o \
$(ODIR)/mctopo3d_table.o \
$(ODIR)/mctree.o \
$(ODIR)/mcunionfind.o \
$(ODIR)/avscrop.o \
$(ODIR)/avsimage.o \
$(ODIR)/llut.o \
$(ODIR)/jclabelextrema.o \
$(ODIR)/llpemeyer4d.o \
$(ODIR)/llpeGA.o \
$(ODIR)/jcgraphes.o \
$(ODIR)/lhierarchie.o \
$(ODIR)/lMSF.o \
$(ODIR)/jccodimage.o \
$(ODIR)/jccomptree.o \
$(ODIR)/lppm2GA.o \
$(ODIR)/lga2khalimsky.o \
$(ODIR)/jcimage.o \
$(ODIR)/ssexport.o \
$(ODIR)/liarwrap.o

# ===============================================================
# EXECUTABLES
# ===============================================================

# *********************************
# ARITH
# *********************************

$(BDIR)/add:	$(CDIR)/add.c $(IDIR)/larith.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/add.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/add

$(BDIR)/addconst:	$(CDIR)/addconst.c $(IDIR)/larith.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/addconst.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/addconst

$(BDIR)/area:	$(CDIR)/area.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/area.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/area

$(BDIR)/argmax:	$(CDIR)/argmax.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/argmax.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/argmax

$(BDIR)/argmin:	$(CDIR)/argmin.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/argmin.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/argmin

$(BDIR)/average:	$(CDIR)/average.c $(IDIR)/larith.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/average.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/average

$(BDIR)/complex_modulus:	$(CDIR)/complex_modulus.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/complex_modulus.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/complex_modulus

$(BDIR)/complex_real:	$(CDIR)/complex_real.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/complex_real.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/complex_real

$(BDIR)/complex_imaginary:	$(CDIR)/complex_imaginary.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/complex_imaginary.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/complex_imaginary

$(BDIR)/correctbias:	$(CDIR)/correctbias.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/correctbias.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/correctbias

$(BDIR)/diffimages:	$(CDIR)/diffimages.c $(IDIR)/larith.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/diffimages.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/diffimages

$(BDIR)/div:	$(CDIR)/div.c $(IDIR)/larith.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/div.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/div

$(BDIR)/dup:	$(CDIR)/dup.c $(IDIR)/larith.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dup.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/dup

$(BDIR)/equal:	$(CDIR)/equal.c $(IDIR)/larith.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/equal.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/equal

$(BDIR)/exp:	$(CDIR)/exp.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/exp.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/exp

$(BDIR)/gammacor:	$(CDIR)/gammacor.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/gammacor.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/gammacor

$(BDIR)/genfield:	$(CDIR)/genfield.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/genfield.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/genfield

$(BDIR)/genimage:	$(CDIR)/genimage.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/genimage.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/genimage

$(BDIR)/genkernel:	$(CDIR)/genkernel.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/genkernel.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/genkernel

$(BDIR)/inf:	$(CDIR)/inf.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/inf.c $(OBJ_COMMON) $(ODIR)/larith.o $(LIBS) -o $(BDIR)/inf

$(BDIR)/inverse:	$(CDIR)/inverse.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/inverse.c $(OBJ_COMMON) $(ODIR)/larith.o $(LIBS) -o $(BDIR)/inverse

$(BDIR)/isnull:	$(CDIR)/isnull.c $(IDIR)/larith.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/isnull.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/isnull

$(BDIR)/log:	$(CDIR)/log.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/log.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/log

$(BDIR)/mask:	$(CDIR)/mask.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/mask.c $(OBJ_COMMON) $(ODIR)/larith.o $(LIBS) -o $(BDIR)/mask

$(BDIR)/max:	$(CDIR)/max.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/max.c $(OBJ_COMMON) $(ODIR)/larith.o $(LIBS) -o $(BDIR)/max

$(BDIR)/max1:	$(CDIR)/max1.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/max1.c $(OBJ_COMMON) $(ODIR)/larith.o $(LIBS) -o $(BDIR)/max1

$(BDIR)/min:	$(CDIR)/min.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/min.c $(OBJ_COMMON) $(ODIR)/larith.o $(LIBS) -o $(BDIR)/min

$(BDIR)/min1:	$(CDIR)/min1.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/min1.c $(OBJ_COMMON) $(ODIR)/larith.o $(LIBS) -o $(BDIR)/min1

$(BDIR)/moyenne:	$(CDIR)/moyenne.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/moyenne.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/moyenne

$(BDIR)/mult:	$(CDIR)/mult.c $(IDIR)/larith.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/mult.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/mult

$(BDIR)/normalize:	$(CDIR)/normalize.c $(IDIR)/larith.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/normalize.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/normalize

$(BDIR)/pow:	$(CDIR)/pow.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pow.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/pow

$(BDIR)/scale:	$(CDIR)/scale.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/scale.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/scale

$(BDIR)/selndg:	$(CDIR)/selndg.c $(IDIR)/mcimage.h $(IDIR)/lselndg.h $(OBJ_COMMON) $(ODIR)/lselndg.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/selndg.c $(OBJ_COMMON) $(ODIR)/lselndg.o $(LIBS) -o $(BDIR)/selndg

$(BDIR)/seuil:	$(CDIR)/seuil.c $(IDIR)/mcimage.h $(IDIR)/lseuil.h $(OBJ_COMMON) $(ODIR)/lseuil.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/seuil.c $(OBJ_COMMON) $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/seuil

$(BDIR)/seuil2:	$(CDIR)/seuil2.c $(IDIR)/mcimage.h $(IDIR)/lseuil.h $(OBJ_COMMON) $(ODIR)/lseuil.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/seuil2.c $(OBJ_COMMON) $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/seuil2

$(BDIR)/seuilauto:	$(CDIR)/seuilauto.c $(IDIR)/mcimage.h $(IDIR)/lseuil.h $(IDIR)/lhisto.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/seuilauto.c $(OBJ_COMMON) $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/seuilauto

$(BDIR)/sub:	$(CDIR)/sub.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/sub.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/sub

$(BDIR)/sup:	$(CDIR)/sup.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/sup.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/sup

$(BDIR)/threshold:	$(CDIR)/threshold.c $(IDIR)/mcimage.h $(IDIR)/lseuil.h $(OBJ_COMMON) $(ODIR)/lseuil.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/threshold.c $(OBJ_COMMON) $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/threshold

$(BDIR)/volume:	$(CDIR)/volume.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/volume.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/volume

$(BDIR)/xor:	$(CDIR)/xor.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/xor.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/xor

# *********************************
# CONVERT
# *********************************
$(BDIR)/2elisp:	$(CDIR)/2elisp.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2elisp.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/2elisp

$(BDIR)/2latex:	$(CDIR)/2latex.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2latex.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/2latex

$(BDIR)/ac2pink:	$(CDIR)/ac2pink.c
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ac2pink.c $(LIBS) -o $(BDIR)/ac2pink

$(BDIR)/any2complex:	$(CDIR)/any2complex.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/any2complex.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/any2complex

$(BDIR)/bmp2ppm:	$(CDIR)/bmp2ppm.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/bmp2ppm.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/bmp2ppm

$(BDIR)/bmp2pgm:	$(CDIR)/bmp2pgm.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/bmp2pgm.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/bmp2pgm

$(BDIR)/byte2float:	$(CDIR)/byte2float.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/byte2float.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/byte2float

$(BDIR)/byte2long:	$(CDIR)/byte2long.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/byte2long.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/byte2long

$(BDIR)/car2pol:	$(CDIR)/car2pol.c $(IDIR)/mcimage.h $(IDIR)/lgeo.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mcgeo.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/lbresen.o $(ODIR)/llabelextrema.o $(ODIR)/lgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/car2pol.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/car2pol

$(BDIR)/catgif:	$(CDIR)/catgif.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/catgif.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/catgif

$(BDIR)/catpgm:	$(CDIR)/catpgm.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/catpgm.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/catpgm

$(BDIR)/changename:	$(CDIR)/changename.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/changename.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/changename

$(BDIR)/concat3d:	$(CDIR)/concat3d.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/concat3d.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/concat3d

$(BDIR)/double2byte:	$(CDIR)/double2byte.c $(IDIR)/mcimage.h $(IDIR)/lhisto.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/double2byte.c $(OBJ_COMMON) $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/double2byte

$(BDIR)/explode:	$(CDIR)/explode.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/explode.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/explode

$(BDIR)/explodecomp:	$(CDIR)/explodecomp.c $(IDIR)/llabelextrema.h $(IDIR)/llabelplateaux.h $(IDIR)/mcimage.h $(ODIR)/llabelextrema.o $(ODIR)/llabelplateaux.o $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/explodecomp.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/explodecomp

$(BDIR)/extrude:	$(CDIR)/extrude.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/extrude.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/extrude

$(BDIR)/float2byte:	$(CDIR)/float2byte.c $(IDIR)/mcimage.h $(IDIR)/lhisto.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/float2byte.c $(OBJ_COMMON) $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/float2byte

$(BDIR)/float2long:	$(CDIR)/float2long.c $(IDIR)/mcimage.h $(IDIR)/lhisto.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/float2long.c $(OBJ_COMMON) $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/float2long

$(BDIR)/graph2pgm:	$(CDIR)/graph2pgm.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcgraphe.h $(ODIR)/mcgraphe.o $(IDIR)/mccodimage.h $(ODIR)/mccodimage.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/graph2pgm.c $(OBJ_COMMON)  $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/graph2pgm

$(BDIR)/graph2ps:	$(CDIR)/graph2ps.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcgraphe.h $(ODIR)/mcgraphe.o $(IDIR)/mccodimage.h $(ODIR)/mccodimage.o $(OBJ_COMMON) 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/graph2ps.c $(OBJ_COMMON)  $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/graph2ps

$(BDIR)/hls2rgb:	$(CDIR)/hls2rgb.c $(IDIR)/mcimage.h $(IDIR)/mccolors.h $(OBJ_COMMON) $(ODIR)/mccolors.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/hls2rgb.c $(OBJ_COMMON) $(ODIR)/mccolors.o $(LIBS) -o $(BDIR)/hls2rgb

$(BDIR)/list2pgm:	$(CDIR)/list2pgm.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/list2pgm.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/list2pgm

$(BDIR)/long2byte:	$(CDIR)/long2byte.c $(IDIR)/mcimage.h $(IDIR)/lhisto.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/long2byte.c $(OBJ_COMMON) $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/long2byte

$(BDIR)/long2float:	$(CDIR)/long2float.c $(IDIR)/mcimage.h $(IDIR)/lhisto.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/long2float.c $(OBJ_COMMON) $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/long2float

$(BDIR)/mhd2pgm:	$(CDIR)/mhd2pgm.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/mhd2pgm.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/mhd2pgm

$(BDIR)/nifti2pgm:	$(CDIR)/nifti2pgm.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/nifti2pgm.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/nifti2pgm

$(BDIR)/ombre:	$(CDIR)/ombre.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ombre.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/ombre

$(BDIR)/pgm2bmp:	$(CDIR)/pgm2bmp.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2bmp.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/pgm2bmp

$(BDIR)/pgm2closedcurve:	$(CDIR)/pgm2closedcurve.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo3d.h $(IDIR)/mctopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2closedcurve.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/pgm2closedcurve

$(BDIR)/pgm2curve:	$(CDIR)/pgm2curve.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo3d.h $(IDIR)/mctopo.h $(IDIR)/lcurves.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lcurves.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2curve.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/pgm2curve

$(BDIR)/pgm2graph:	$(CDIR)/pgm2graph.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcgraphe.h $(ODIR)/mcgraphe.o $(IDIR)/mccodimage.h $(ODIR)/mccodimage.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2graph.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/pgm2graph

$(BDIR)/pgm2list:	$(CDIR)/pgm2list.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2list.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/pgm2list

$(BDIR)/pgm2pov:	$(CDIR)/pgm2pov.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2pov.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/pgm2pov

$(BDIR)/pgm2ppm:	$(CDIR)/pgm2ppm.c $(IDIR)/mcimage.h $(OBJ_COMMON) 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2ppm.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/pgm2ppm

$(BDIR)/pgm2ps:	$(CDIR)/pgm2ps.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mckhalimsky2d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2ps.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/pgm2ps

$(BDIR)/pgm2raw:	$(CDIR)/pgm2raw.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2raw.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/pgm2raw

$(BDIR)/pgm2slc:	$(CDIR)/pgm2slc.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2slc.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/pgm2slc

$(BDIR)/pgm2vtk:	$(CDIR)/pgm2vtk.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2vtk.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/pgm2vtk

$(BDIR)/pgmascmode:	$(CDIR)/pgmascmode.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgmascmode.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/pgmascmode

$(BDIR)/pgmrawmode:	$(CDIR)/pgmrawmode.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgmrawmode.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/pgmrawmode

$(BDIR)/pol2car:	$(CDIR)/pol2car.c $(IDIR)/mcimage.h $(IDIR)/lgeo.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mclin.o $(ODIR)/mcgeo.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/llabelextrema.o $(ODIR)/lgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pol2car.c $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mcgeo.o $(ODIR)/mclin.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/llabelextrema.o $(ODIR)/lgeo.o $(LIBS) -o $(BDIR)/pol2car

$(BDIR)/ppm2bmp:	$(CDIR)/ppm2bmp.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ppm2bmp.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/ppm2bmp

$(BDIR)/ppm2pgm:	$(CDIR)/ppm2pgm.c $(IDIR)/mcimage.h $(OBJ_COMMON) 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ppm2pgm.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/ppm2pgm

$(BDIR)/ppmascmode:	$(CDIR)/ppmascmode.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ppmascmode.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/ppmascmode

$(BDIR)/raw2pgm:	$(CDIR)/raw2pgm.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/raw2pgm.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/raw2pgm

$(BDIR)/rawthreshold:	$(CDIR)/rawthreshold.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/rawthreshold.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/rawthreshold

$(BDIR)/readgif:	$(CDIR)/readgif.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/readgif.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/readgif

$(BDIR)/reformat:	$(CDIR)/reformat.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/reformat.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/reformat

$(BDIR)/rgb2hls:	$(CDIR)/rgb2hls.c $(IDIR)/mcimage.h $(IDIR)/mccolors.h $(OBJ_COMMON) $(ODIR)/mccolors.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/rgb2hls.c $(OBJ_COMMON) $(ODIR)/mccolors.o $(LIBS) -o $(BDIR)/rgb2hls

$(BDIR)/rgb2ppm:	$(CDIR)/rgb2ppm.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/rgb2ppm.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/rgb2ppm

$(BDIR)/sceneconvert:	$(CDIR)/sceneconvert.c $(IDIR)/mcmesh.h $(IDIR)/mciomesh.h $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(ODIR)/mcgeo.o $(ODIR)/mcprobas.o $(OBJ_COMMON)
			$(CPP) $(CCFLAGS) -I$(IDIR) -I. $(CDIR)/sceneconvert.c $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(ODIR)/mcgeo.o $(ODIR)/mcprobas.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/sceneconvert

$(BDIR)/short2long:	$(CDIR)/short2long.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/short2long.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/short2long

$(BDIR)/short2float:	$(CDIR)/short2float.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/short2float.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/short2float

$(BDIR)/ppm2GA:         $(CDIR)/ppm2GA.c $(IDIR)/jcimage.h $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lppm2GA.h $(IDIR)/jclderiche.h $(ODIR)/jcimage.o $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/lppm2GA.o $(ODIR)/lderiche.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ppm2GA.c $(ODIR)/lderiche.o $(ODIR)/jcimage.o $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/lppm2GA.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/ppm2GA 

$(BDIR)/pgm2GA:		 $(CDIR)/pgm2GA.c $(IDIR)/jcimage.h $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lppm2GA.h $(IDIR)/jclderiche.h $(ODIR)/jcimage.o $(ODIR)/mccodimage.o $(ODIR)/lppm2GA.o $(ODIR)/lderiche.o $(ODIR)/jccodimage.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2GA.c $(ODIR)/lderiche.o $(ODIR)/jcimage.o $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/lppm2GA.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/pgm2GA 

$(BDIR)/pgm2GA4d:	$(CDIR)/pgm2GA4d.c $(IDIR)/mcimage.h $(IDIR)/jcimage.h $(IDIR)/mccodimage.h $(IDIR)/jccodimage.h $(IDIR)/lppm2GA.h $(ODIR)/jcimage.o $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/lppm2GA.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2GA4d.c $(ODIR)/lderiche.o $(ODIR)/jcimage.o $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/lppm2GA.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/pgm2GA4d 

$(BDIR)/GA2pgm:	$(CDIR)/GA2pgm.c $(IDIR)/mcimage.h $(IDIR)/jcimage.h $(IDIR)/mccodimage.h $(IDIR)/jccodimage.h $(IDIR)/lppm2GA.h $(ODIR)/jcimage.o $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/lppm2GA.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/GA2pgm.c $(ODIR)/lderiche.o $(ODIR)/jcimage.o $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/lppm2GA.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/GA2pgm 

$(BDIR)/GA2khalimsky:	 $(CDIR)/GA2khalimsky.c $(IDIR)/mcimage.h $(IDIR)/jcimage.h $(IDIR)/mccodimage.h $(IDIR)/lga2khalimsky.h $(ODIR)/jcimage.o $(ODIR)/mccodimage.o $(ODIR)/lga2khalimsky.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/GA2khalimsky.c $(ODIR)/jcimage.o $(ODIR)/mccodimage.o $(ODIR)/lga2khalimsky.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/GA2khalimsky

$(BDIR)/GA2tikz:	 $(CDIR)/GA2tikz.c $(IDIR)/mcimage.h $(IDIR)/jcimage.h $(IDIR)/mccodimage.h $(ODIR)/jcimage.o $(ODIR)/mccodimage.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/GA2tikz.c $(ODIR)/jcimage.o $(ODIR)/mccodimage.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/GA2tikz

$(BDIR)/section:	$(CDIR)/section.c $(IDIR)/mcimage.h $(IDIR)/lgeo.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mclin.o $(ODIR)/mcgeo.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/llabelextrema.o $(ODIR)/lgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/section.c $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mcgeo.o $(ODIR)/mclin.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/llabelextrema.o $(ODIR)/lgeo.o $(LIBS) -o $(BDIR)/section

$(BDIR)/setorigin:	$(CDIR)/setorigin.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/setorigin.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/setorigin

$(BDIR)/setvoxdim:	$(CDIR)/setvoxdim.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/setvoxdim.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(LIBS) -o $(BDIR)/setvoxdim

$(BDIR)/surimp:	$(CDIR)/surimp.c $(IDIR)/larith.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/surimp.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/surimp

$(BDIR)/yuv2rgb:	$(CDIR)/yuv2rgb.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/yuv2rgb.c $(OBJ_COMMON) $(LIBS) -o $(BDIR)/yuv2rgb

# *********************************
# MORPHO
# *********************************
$(BDIR)/asf:	$(CDIR)/asf.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/asf.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/asf

$(BDIR)/asfbin:	$(CDIR)/asfbin.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldist.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/asfbin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/asfbin

$(BDIR)/asflin:	$(CDIR)/asflin.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/asflin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/asflin

$(BDIR)/asfr:	$(CDIR)/asfr.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(IDIR)/lgeodesic.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o $(ODIR)/lgeodesic.o $(ODIR)/mcfifo.o $(ODIR)/mcindic.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/asfr.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/asfr

$(BDIR)/closeball:	$(CDIR)/closeball.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldist.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/closeball.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/closeball

$(BDIR)/closing:	$(CDIR)/closing.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(IDIR)/lsym.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o $(ODIR)/lsym.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/closing.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/closing

$(BDIR)/deletecomp:	$(CDIR)/deletecomp.c $(IDIR)/mccodimage.h $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/lgeodesic.h $(ODIR)/mccodimage.o $(OBJ_COMMON) $(ODIR)/mcindic.o $(ODIR)/lgeodesic.o $(ODIR)/mcfifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/deletecomp.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/deletecomp

$(BDIR)/dilat:	$(CDIR)/dilat.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dilat.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/dilat

$(BDIR)/dilatball:	$(CDIR)/dilatball.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldist.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dilatball.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/dilatball

$(BDIR)/dilatballnum:	$(CDIR)/dilatballnum.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldilateros.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dilatballnum.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/dilatballnum

$(BDIR)/dilatbin3d:	$(CDIR)/dilatbin3d.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros3d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dilatbin3d.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/dilatbin3d

$(BDIR)/dilatbin:	$(CDIR)/dilatbin.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dilatbin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/dilatbin

$(BDIR)/dilatfast:	$(CDIR)/dilatfast.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dilatfast.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/dilatfast

$(BDIR)/dilation:	$(CDIR)/dilation.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dilation.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/dilation

$(BDIR)/dilatnum:	$(CDIR)/dilatnum.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dilatnum.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/dilatnum

$(BDIR)/dir:	$(CDIR)/dir.c $(IDIR)/ldir.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcindic.o $(ODIR)/ldir.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dir.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/dir

$(BDIR)/dist:	$(CDIR)/dist.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldist.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dist.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/dist

$(BDIR)/distc:	$(CDIR)/distc.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldist.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/distc.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/distc

$(BDIR)/distgeo:	$(CDIR)/distgeo.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldistgeo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldistgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/distgeo.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/distgeo

$(BDIR)/distsets:	$(CDIR)/distsets.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldist.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/distsets.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/distsets

$(BDIR)/dynamique:	$(CDIR)/dynamique.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccomptree.h $(IDIR)/llabelextrema.h $(IDIR)/mccodimage.h $(IDIR)/ldynamique.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccomptree.o $(ODIR)/mcunionfind.o $(ODIR)/llabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/ldynamique.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dynamique.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/dynamique

$(BDIR)/dynamiquefilter:	$(CDIR)/dynamiquefilter.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccomptree.h $(IDIR)/llabelextrema.h $(IDIR)/mccodimage.h $(IDIR)/ldynamique.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccomptree.o $(ODIR)/mcunionfind.o $(ODIR)/llabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/ldynamique.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dynamiquefilter.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/dynamiquefilter

$(BDIR)/eros:	$(CDIR)/eros.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/eros.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/eros

$(BDIR)/erosball:	$(CDIR)/erosball.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldist.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/erosball.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/erosball

$(BDIR)/erosballnum:	$(CDIR)/erosballnum.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldilateros.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/erosballnum.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/erosballnum

$(BDIR)/erosbin:	$(CDIR)/erosbin.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/erosbin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/erosbin

$(BDIR)/erosbin3d:	$(CDIR)/erosbin3d.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros3d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/erosbin3d.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/erosbin3d

$(BDIR)/erosfast:	$(CDIR)/erosfast.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/erosfast.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/erosfast

$(BDIR)/erosfast3d:	$(CDIR)/erosfast3d.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros3d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/erosfast3d.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/erosfast3d

$(BDIR)/erosion:	$(CDIR)/erosion.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/erosion.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/erosion

$(BDIR)/erosnum:	$(CDIR)/erosnum.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/erosnum.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/erosnum

$(BDIR)/filtreordre:	$(CDIR)/filtreordre.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lfiltreordre.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lfiltreordre.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/filtreordre.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/filtreordre

$(BDIR)/filtrerang:	$(CDIR)/filtrerang.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lfiltreordre.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lfiltreordre.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/filtrerang.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/filtrerang

$(BDIR)/fmm: $(CDIR)/fmm.c $(IDIR)/lfmm.h $(IDIR)/pde_toolbox.h $(IDIR)/pde_toolbox_defs.h $(ODIR)/lfmm.o $(ODIR)/fmmgeodist.o $(ODIR)/lfmmdist.o $(ODIR)/bimage.o $(ODIR)/bimage_utils.o $(ODIR)/lstb_io.o
	     $(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/fmm.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/fmm

$(BDIR)/granulometry:	$(CDIR)/granulometry.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldist.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/granulometry.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/granulometry

$(BDIR)/hitormiss:	$(CDIR)/hitormiss.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(IDIR)/larith.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/hitormiss.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/hitormiss

$(BDIR)/lintophat:	$(CDIR)/lintophat.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lintophat.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lintophat

$(BDIR)/medianfilter:	$(CDIR)/medianfilter.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lfiltreordre.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lfiltreordre.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/medianfilter.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/medianfilter

$(BDIR)/medialaxis:	$(CDIR)/medialaxis.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lmedialaxis.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/medialaxis.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/medialaxis

$(BDIR)/integermedialaxis:	$(CDIR)/integermedialaxis.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lvoronoilabelling.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lvoronoilabelling.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/integermedialaxis.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/integermedialaxis

$(BDIR)/lambdamedialaxis:	$(CDIR)/lambdamedialaxis.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lmedialaxis.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lambdamedialaxis.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lambdamedialaxis

$(BDIR)/lambdamedialaxisauto:	$(CDIR)/lambdamedialaxisauto.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lmedialaxis.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lambdamedialaxisauto.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lambdamedialaxisauto

$(BDIR)/lambdamedialaxis2:	$(CDIR)/lambdamedialaxis2.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lmedialaxis.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lambdamedialaxis2.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lambdamedialaxis2

$(BDIR)/lambdamedialaxis2auto:	$(CDIR)/lambdamedialaxis2auto.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lmedialaxis.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lambdamedialaxis2auto.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lambdamedialaxis2auto

$(BDIR)/lambdapmedialaxis:	$(CDIR)/lambdapmedialaxis.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lmedialaxis.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/lvoronoilabelling.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lambdapmedialaxis.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lambdapmedialaxis

$(BDIR)/lantuejoul:	$(CDIR)/lantuejoul.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldist.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lantuejoul.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lantuejoul

$(BDIR)/openball:	$(CDIR)/openball.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldist.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/openball.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/openball

$(BDIR)/opening:	$(CDIR)/opening.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(IDIR)/lsym.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o $(ODIR)/lsym.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/opening.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/opening

$(BDIR)/openingfunction:	$(CDIR)/openingfunction.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lmedialaxis.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/lmedialaxis.o $(ODIR)/lballincl.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/ltopotypes.o $(ODIR)/ldist.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/openingfunction.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/openingfunction

$(BDIR)/radialopening:	 $(CDIR)/radialopening.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/larith.h $(IDIR)/lsym.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/larith.o $(ODIR)/lsym.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o $(ODIR)/lbresen.o $(ODIR)/ldraw.o
	 $(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/radialopening.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/radialopening

$(BDIR)/radialrankmaxopening:	 $(CDIR)/radialrankmaxopening.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lfiltreordre.h $(IDIR)/larith.h $(IDIR)/lsym.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/larith.o $(ODIR)/lsym.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/lfiltreordre.o
	 $(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/radialrankmaxopening.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/radialrankmaxopening

$(BDIR)/rankfilter:	$(CDIR)/rankfilter.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lfiltreordre.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lfiltreordre.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/rankfilter.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/rankfilter

$(BDIR)/rankmaxopening:	 $(CDIR)/rankmaxopening.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/larith.h $(IDIR)/lfiltreordre.h $(IDIR)/lsym.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/larith.o $(ODIR)/lsym.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o $(ODIR)/lfiltreordre.o
	 $(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/rankmaxopening.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/rankmaxopening

$(BDIR)/redt:	$(CDIR)/redt.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldist.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/redt.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/redt

$(BDIR)/scaleaxis:	$(CDIR)/scaleaxis.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lmedialaxis.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/scaleaxis.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/scaleaxis

$(BDIR)/scalefilteredmedialaxis:	$(CDIR)/scalefilteredmedialaxis.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lmedialaxis.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/scalefilteredmedialaxis.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/scalefilteredmedialaxis

$(BDIR)/sfema:	$(CDIR)/sfema.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lmedialaxis.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/sfema.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/sfema

$(BDIR)/ultimateerosion:	$(CDIR)/ultimateerosion.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldist.h $(IDIR)/lgeodesic.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mcindic.o $(ODIR)/ldist.o $(ODIR)/lgeodesic.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ultimateerosion.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ultimateerosion

$(BDIR)/voronoilabelling:	$(CDIR)/voronoilabelling.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lvoronoilabelling.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/lvoronoilabelling.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/voronoilabelling.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/voronoilabelling


# *********************************
# CONNECT
# *********************************

$(BDIR)/amont:	$(CDIR)/amont.c $(IDIR)/mccodimage.h $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/lgeodesic.h $(IDIR)/larith.h $(ODIR)/mccodimage.o $(OBJ_COMMON) $(ODIR)/mcindic.o $(ODIR)/lgeodesic.o $(ODIR)/larith.o $(ODIR)/mcfifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/amont.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/amont

$(BDIR)/areaclosing:	$(CDIR)/areaclosing.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/lattribarea.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/lattribarea.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/areaclosing.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/areaclosing

$(BDIR)/areaopening:	$(CDIR)/areaopening.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/lattribarea.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/lattribarea.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/areaopening.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/areaopening

$(BDIR)/areaselnb:	$(CDIR)/areaselnb.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/lattribarea.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/lattribarea.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/areaselnb.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/areaselnb

$(BDIR)/geodilat:	$(CDIR)/geodilat.c $(IDIR)/mccodimage.h $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/lgeodesic.h $(ODIR)/mccodimage.o $(OBJ_COMMON) $(ODIR)/mcindic.o $(ODIR)/lgeodesic.o $(ODIR)/mcfifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/geodilat.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/geodilat

$(BDIR)/geoeros:	$(CDIR)/geoeros.c $(IDIR)/mccodimage.h $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/lgeodesic.h $(ODIR)/mccodimage.o $(OBJ_COMMON) $(ODIR)/mcindic.o $(ODIR)/lgeodesic.o $(ODIR)/mcfifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/geoeros.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/geoeros

$(BDIR)/heightmaxima:	$(CDIR)/heightmaxima.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/lattribheight.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/lattribheight.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/heightmaxima.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/heightmaxima

$(BDIR)/heightminima:	$(CDIR)/heightminima.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/lattribheight.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/lattribheight.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/heightminima.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/heightminima

$(BDIR)/heightselnb:	$(CDIR)/heightselnb.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/lattribheight.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/lattribheight.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/heightselnb.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/heightselnb

$(BDIR)/label:	$(CDIR)/label.c $(IDIR)/mcimage.h $(IDIR)/llabelplateaux.h $(IDIR)/llabelextrema.h $(OBJ_COMMON) $(ODIR)/llabelplateaux.o $(ODIR)/llabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/larith.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/label.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/label

$(BDIR)/labeldil:	$(CDIR)/labeldil.c $(IDIR)/mcimage.h $(IDIR)/llabelplateaux.h $(IDIR)/llabelextrema.h $(OBJ_COMMON) $(ODIR)/llabelplateaux.o $(ODIR)/llabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/labeldil.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/labeldil

$(BDIR)/labelfgd:	$(CDIR)/labelfgd.c $(IDIR)/mcimage.h $(IDIR)/llabelplateaux.h $(IDIR)/llabelextrema.h $(OBJ_COMMON) $(ODIR)/llabelplateaux.o $(ODIR)/llabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/larith.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/labelfgd.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/labelfgd

$(BDIR)/nbcomp:	$(CDIR)/nbcomp.c $(IDIR)/mcimage.h $(IDIR)/llabelplateaux.h $(IDIR)/llabelextrema.h $(OBJ_COMMON) $(ODIR)/llabelplateaux.o $(ODIR)/llabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/nbcomp.c $(ODIR)/libpink.a $(LIBS) -o	$(BDIR)/nbcomp

$(BDIR)/propgeo:	$(CDIR)/propgeo.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/mcprobas.h $(IDIR)/lpropgeo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcindic.o $(ODIR)/mclifo.o $(ODIR)/mcprobas.o $(ODIR)/lpropgeo.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/propgeo.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/propgeo

$(BDIR)/segment3d:	$(CDIR)/segment3d.c $(IDIR)/mcimage.h $(IDIR)/llabelextrema.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mccbt.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lsegment3d.h $(OBJ_COMMON) $(ODIR)/llabelextrema.o $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccbt.o $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/lsegment3d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/segment3d.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/segment3d

$(BDIR)/segmentarea:	$(CDIR)/segmentarea.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/lattribarea.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/lattribarea.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/segmentarea.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/segmentarea

$(BDIR)/segmentheight:	$(CDIR)/segmentheight.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/lattribarea.h $(IDIR)/lsegreconsheight.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/lattribheight.o $(ODIR)/lsegreconsheight.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/segmentheight.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/segmentheight

$(BDIR)/segmentnum:	$(CDIR)/segmentnum.c $(IDIR)/mcimage.h $(IDIR)/llabelplateaux.h $(IDIR)/mclifo.h $(IDIR)/mcfah.h $(IDIR)/mccbt.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lsegmentnum.h $(OBJ_COMMON) $(ODIR)/llabelplateaux.o $(ODIR)/mclifo.o $(OBJMCFAH) $(ODIR)/mccbt.o $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/lsegmentnum.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/segmentnum.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/segmentnum

$(BDIR)/segmentvol:	$(CDIR)/segmentvol.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/lattribvol.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/lattribvol.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/segmentvol.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/segmentvol

$(BDIR)/selectcomp:	$(CDIR)/selectcomp.c $(IDIR)/mccodimage.h $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/lgeodesic.h $(IDIR)/larith.h $(ODIR)/mccodimage.o $(OBJ_COMMON) $(ODIR)/mcindic.o $(ODIR)/lgeodesic.o $(ODIR)/larith.o $(ODIR)/mcfifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/selectcomp.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/selectcomp

$(BDIR)/shortestpath:	$(CDIR)/shortestpath.c $(IDIR)/mccodimage.h $(ODIR)/mccodimage.o $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcgraphe.h $(ODIR)/mcgraphe.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/shortestpath.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/shortestpath

$(BDIR)/volmaxima:	$(CDIR)/volmaxima.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/lattribvol.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/lattribvol.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/volmaxima.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/volmaxima

$(BDIR)/volselnb:	$(CDIR)/volselnb.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/lattribvol.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/lattribvol.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/volselnb.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/volselnb

$(BDIR)/watershed:	$(CDIR)/watershed.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/mccomptree.h $(IDIR)/lwshedtopo.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/mccomptree.o $(ODIR)/mcunionfind.o $(ODIR)/lwshedtopo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/watershed.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/watershed

$(BDIR)/watershedMeyer:	$(CDIR)/watershedMeyer.c $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mccodimage.h $(IDIR)/llpemeyer.h $(OBJ_COMMON) $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/llpemeyer.o $(ODIR)/mckhalimsky2d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/watershedMeyer.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/watershedMeyer

$(BDIR)/saliency:	$(CDIR)/saliency.c $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mccodimage.h $(IDIR)/llpemeyer.h $(OBJ_COMMON) $(ODIR)/mcindic.o $(ODIR)/mccodimage.o $(ODIR)/llpemeyer.o $(ODIR)/mckhalimsky2d.o $(ODIR)/lsaliency.o $(ODIR)/llabelextrema.o
	$(CC) $(CCFLAGS) -I$(IDIR) -c $(LDIR)/mclistechainee.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/saliency

$(BDIR)/MSF:	$(CDIR)/MSF.c $(ODIR)/jcimage.o $(ODIR)/jclabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mclifo.o $(ODIR)/lMSF.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/MSF.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/MSF

$(BDIR)/MSF4d:	$(CDIR)/MSF4d.c $(ODIR)/jcimage.o $(ODIR)/jclabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mclifo.o $(ODIR)/lMSF.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/MSF4d.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/MSF4d

$(BDIR)/GAwatershed:	$(CDIR)/GAwatershed.c $(ODIR)/jcimage.o $(ODIR)/jclabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/llpeGA.o $(ODIR)/jcgraphes.o $(ODIR)/mcfifo.o $(ODIR)/mclifo.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/GAwatershed.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/GAwatershed 

$(BDIR)/GAwatershedDouble:	 $(CDIR)/GAwatershedDouble.c $(ODIR)/jcimage.o $(ODIR)/jclabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/llpeGA.o $(ODIR)/jcgraphes.o $(ODIR)/mcfifo.o $(ODIR)/mclifo.o $(ODIR)/lppm2GA.o $(ODIR)/lderiche.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/GAwatershedDouble.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/GAwatershedDouble 

$(BDIR)/watershedMeyer4D:	$(CDIR)/watershedMeyer4D.c $(ODIR)/jcimage.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/llpemeyer4d.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/watershedMeyer4D.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/watershedMeyer4D

$(BDIR)/watershedthin:	$(CDIR)/watershedthin.c $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mccodimage.h $(IDIR)/llpemeyer.h $(OBJ_COMMON) $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/llpemeyer.o $(ODIR)/mckhalimsky2d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/watershedthin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/watershedthin

$(BDIR)/watershedwithoutline:	$(CDIR)/watershedwithoutline.c $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mccodimage.h $(IDIR)/llpemeyer.h $(OBJ_COMMON) $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/llpemeyer.o $(ODIR)/mckhalimsky2d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/watershedwithoutline.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/watershedwithoutline

$(BDIR)/watershedwithoutlinelab:	$(CDIR)/watershedwithoutlinelab.c $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mccodimage.h $(IDIR)/llpemeyer.h $(OBJ_COMMON) $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/llpemeyer.o $(ODIR)/mckhalimsky2d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/watershedwithoutlinelab.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/watershedwithoutlinelab

$(BDIR)/watershedMeyer2:	$(CDIR)/watershedMeyer2.c $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mccodimage.h $(IDIR)/llpemeyer.h $(OBJ_COMMON) $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/llpemeyer.o $(ODIR)/mckhalimsky2d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/watershedMeyer2.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/watershedMeyer2

$(BDIR)/watershedMeyer2lab:	$(CDIR)/watershedMeyer2lab.c $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mccodimage.h $(IDIR)/llpemeyer.h $(OBJ_COMMON) $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/llpemeyer.o $(ODIR)/mckhalimsky2d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/watershedMeyer2lab.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/watershedMeyer2lab

$(BDIR)/watershedMeyer3:	$(CDIR)/watershedMeyer3.c $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mccodimage.h $(IDIR)/llpemeyer.h $(OBJ_COMMON) $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/llpemeyer.o $(ODIR)/mckhalimsky2d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/watershedMeyer3.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/watershedMeyer3

$(BDIR)/wshedtopo:	$(CDIR)/wshedtopo.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/mccomptree.h $(IDIR)/lwshedtopo.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/mccomptree.o $(ODIR)/mcunionfind.o $(ODIR)/lwshedtopo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/wshedtopo.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/wshedtopo

$(BDIR)/wshedkhalimsky:	$(CDIR)/wshedkhalimsky.c $(OBJMCFAH) $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mccodimage.h $(IDIR)/llpemeyer.h $(IDIR)/mckhalimsky2d.h $(OBJ_COMMON) $(ODIR)/mcindic.o $(ODIR)/mccodimage.o $(ODIR)/llpemeyer.o $(ODIR)/mckhalimsky2d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/wshedkhalimsky.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/wshedkhalimsky

$(BDIR)/wshedval:	$(CDIR)/wshedval.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/ldynamique_grimaud.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/ldynamique_grimaud.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/wshedval.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/wshedval

$(BDIR)/saliencyGA:	$(ODIR)/jcimage.o $(ODIR)/jclabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/llpeGA.o $(ODIR)/jcgraphes.o $(ODIR)/lhierarchie.o $(ODIR)/mcunionfind.o $(ODIR)/jccomptree.o $(ODIR)/mcfifo.o $(ODIR)/mclifo.o $(CDIR)/saliencyGA.c $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/saliencyGA.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/saliencyGA

$(BDIR)/waterfall:	$(ODIR)/jcimage.o $(ODIR)/jclabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/jccodimage.o $(ODIR)/llpeGA.o $(ODIR)/jcgraphes.o $(ODIR)/lhierarchie.o $(ODIR)/mcfifo.o $(CDIR)/waterfall.c $(ODIR)/mcunionfind.o $(ODIR)/jccomptree.o $(ODIR)/mclifo.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/waterfall.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/waterfall 

# *********************************
# TOPO
# *********************************

$(BDIR)/alpha:	$(CDIR)/alpha.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/ltopotypes.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/ltopotypes.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/alpha.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/alpha

$(BDIR)/asft:	$(CDIR)/asft.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lasft.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o $(ODIR)/lhtkern.o $(ODIR)/lhtkern3d.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(OBJMCFAH) $(ODIR)/mcindic.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/lasft.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/asft.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/asft

$(BDIR)/asftmed:	$(CDIR)/asftmed.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lasft.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o $(ODIR)/lhtkern.o $(ODIR)/lhtkern3d.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(OBJMCFAH) $(ODIR)/mcindic.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/lasft.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/asftmed.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/asftmed

$(BDIR)/asftndg:	$(CDIR)/asftndg.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lasft.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o $(ODIR)/lhtkern.o $(ODIR)/lhtkern3d.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(OBJMCFAH) $(ODIR)/mcindic.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/lasft.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/asftndg.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/asftndg

$(BDIR)/border:	$(CDIR)/border.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/lborder.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lborder.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/border.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/border

$(BDIR)/crestheight:	$(CDIR)/crestheight.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(IDIR)/lgeodesic.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/crestheight.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/crestheight

$(BDIR)/crestrestoration:	$(CDIR)/crestrestoration.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/mcfah.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/crestrestoration.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/crestrestoration

$(BDIR)/crestsegment:	$(CDIR)/crestsegment.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(IDIR)/lgeodesic.h $(OBJ_COMMON) $(ODIR)/llabelextrema.o $(ODIR)/lhtkern.o $(ODIR)/lgeodesic.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/crestsegment.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/crestsegment

$(BDIR)/crestvol:	$(CDIR)/crestvol.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(IDIR)/lgeodesic.h $(OBJ_COMMON) $(ODIR)/llabelextrema.o $(ODIR)/lhtkern.o $(ODIR)/lgeodesic.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/crestvol.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/crestvol

$(BDIR)/deletepeaks:	$(CDIR)/deletepeaks.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/lfiltrestopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(OBJMCFAH) $(ODIR)/mcindic.o $(ODIR)/llabelextrema.o $(ODIR)/llambdakern.o $(ODIR)/lfiltrestopo.o $(ODIR)/lhtkern.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/deletepeaks.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/deletepeaks

$(BDIR)/deletewells:	$(CDIR)/deletewells.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/lfiltrestopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(OBJMCFAH) $(ODIR)/mcindic.o $(ODIR)/llabelextrema.o $(ODIR)/llambdakern.o $(ODIR)/lfiltrestopo.o $(ODIR)/lhtkern.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/deletewells.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/deletewells

$(BDIR)/gettree:	$(CDIR)/gettree.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lgettree.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/lgettree.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/gettree.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/gettree

$(BDIR)/grayskel:	$(CDIR)/grayskel.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/llambdakern.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/grayskel.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/grayskel

$(BDIR)/hcutting:	$(CDIR)/hcutting.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lasft.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o $(ODIR)/lhtkern.o $(ODIR)/lhtkern3d.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(OBJMCFAH) $(ODIR)/mcindic.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/lasft.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/hcutting.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/hcutting

$(BDIR)/hdilatball:	$(CDIR)/hdilatball.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/hdilatball.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/hdilatball

$(BDIR)/herosball:	$(CDIR)/herosball.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/herosball.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/herosball

$(BDIR)/hfilling:	$(CDIR)/hfilling.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lasft.h $(IDIR)/ldilateros.h $(IDIR)/ldilateros3d.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/ldilateros.o $(ODIR)/ldilateros3d.o $(ODIR)/lhtkern.o $(ODIR)/lhtkern3d.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(OBJMCFAH) $(ODIR)/mcindic.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/lasft.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/hfilling.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/hfilling

$(BDIR)/hthick:	$(CDIR)/hthick.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/mcfah.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/hthick.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/hthick

$(BDIR)/hthickbin:	$(CDIR)/hthickbin.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfifo.h $(IDIR)/lsquelbin.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mcindic.o $(ODIR)/mcfifo.o $(ODIR)/lsquelbin.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/hthickbin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/hthickbin

$(BDIR)/hthin:	$(CDIR)/hthin.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/mcfah.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/hthin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/hthin

$(BDIR)/hthinbin:	$(CDIR)/hthinbin.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfifo.h $(IDIR)/lsquelbin.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mcindic.o $(ODIR)/mcfifo.o $(ODIR)/lsquelbin.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/hthinbin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/hthinbin

$(BDIR)/hthiniso:	$(CDIR)/hthiniso.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lhthiniso.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/lhthiniso.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/hthiniso.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/hthiniso

$(BDIR)/hthinpar:	$(CDIR)/hthinpar.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo.o $(ODIR)/lskelpar.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/hthinpar.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/hthinpar

$(BDIR)/htkern:	$(CDIR)/htkern.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/htkern.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/htkern

$(BDIR)/htkern3dbin:	$(CDIR)/htkern3dbin.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo3d.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/htkern3dbin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/htkern3dbin

$(BDIR)/htkernu:	$(CDIR)/htkernu.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/htkernu.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/htkernu

$(BDIR)/htkernu3d:	$(CDIR)/htkernu3d.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo3d.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/htkernu3d.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/htkernu3d

$(BDIR)/holeclosing:	$(CDIR)/holeclosing.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo3d.h $(IDIR)/mcfahpure.h $(IDIR)/mcindic.h $(IDIR)/ldist.h $(IDIR)/llabelextrema.h $(IDIR)/lfermetrous3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo3d_table.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(OBJMCFAH) $(ODIR)/ldist.o $(ODIR)/llabelextrema.o $(ODIR)/mcindic.o $(ODIR)/lfermetrous3d.o $(ODIR)/lcrop.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/holeclosing.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/holeclosing

$(BDIR)/labeltree:	$(CDIR)/labeltree.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/llabeltree.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/llabeltree.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/labeltree.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/labeltree

$(BDIR)/lambdalthin:	$(CDIR)/lambdalthin.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/llambdakern.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -DNIVEL -I$(IDIR) $(CDIR)/lambdalthin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lambdalthin

$(BDIR)/lambdathin:	$(CDIR)/lambdathin.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/llambdakern.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lambdathin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lambdathin

$(BDIR)/lambdaskel:	$(CDIR)/lambdaskel.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/llambdakern.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lambdaskel.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lambdaskel

$(BDIR)/kern:	$(CDIR)/kern.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfahpure.h $(IDIR)/llabelextrema.h $(IDIR)/mclifo.h $(IDIR)/lkern.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/llabelextrema.o $(ODIR)/mclifo.o $(ODIR)/lkern.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/kern.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/kern

$(BDIR)/lpetopo:	$(CDIR)/lpetopo.c $(IDIR)/mcimage.h $(IDIR)/llabelextrema.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mccbt.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/llpetopo.h $(OBJ_COMMON) $(ODIR)/llabelextrema.o $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccbt.o $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/llpetopo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lpetopo.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lpetopo

$(BDIR)/lpetoporeg:	$(CDIR)/lpetoporeg.c $(IDIR)/mcimage.h $(IDIR)/llabelextrema.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mccbt.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/llpetoporeg.h $(OBJ_COMMON) $(ODIR)/llabelextrema.o $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccbt.o $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/llpetoporeg.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lpetoporeg.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lpetoporeg

$(BDIR)/lthin:	$(CDIR)/lthin.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/mcfah.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lthin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lthin

$(BDIR)/lthick:	$(CDIR)/lthick.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/mcfah.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lthick.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lthick

$(BDIR)/lvkern:	$(CDIR)/lvkern.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lvkern.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lvkern

$(BDIR)/lvkernu:	$(CDIR)/lvkernu.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lvkernu.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lvkernu

$(BDIR)/maxima:	$(CDIR)/maxima.c $(IDIR)/llabelextrema.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mccodimage.h $(ODIR)/llabelextrema.o $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/maxima.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/maxima

$(BDIR)/minima:	$(CDIR)/minima.c $(IDIR)/lminima.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mccodimage.h $(IDIR)/llabelextrema.h $(ODIR)/llabelextrema.o $(ODIR)/lminima.o $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/minima.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/minima

$(BDIR)/minimalsimplepair:	$(CDIR)/minimalsimplepair.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/minimalsimplepair.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/minimalsimplepair

$(BDIR)/localextrema:	$(CDIR)/localextrema.c $(IDIR)/llocalextrema.h $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(ODIR)/llocalextrema.o $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/localextrema.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/localextrema

$(BDIR)/nbtopo:	$(CDIR)/nbtopo.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/lnbtopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lnbtopo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/nbtopo.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/nbtopo

$(BDIR)/nbvois:	$(CDIR)/nbvois.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lnbvois.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lnbvois.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/nbvois.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/nbvois

$(BDIR)/pgm2skel:	$(CDIR)/pgm2skel.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelcurv.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/lskelcurv.o $(ODIR)/llabelextrema.o $(ODIR)/lseltopo.o $(ODIR)/lmoments.o $(ODIR)/mclin.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mcsplines.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2skel.c $(OBJ_COMMON) $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/lseltopo.o $(ODIR)/mclin.o $(ODIR)/lmoments.o $(ODIR)/llabelextrema.o $(ODIR)/lskelcurv.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mcsplines.o $(LIBS) -o $(BDIR)/pgm2skel

$(BDIR)/pgm2skel2:	$(CDIR)/pgm2skel2.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelcurv.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/lskelcurv.o $(ODIR)/llabelextrema.o $(ODIR)/lseltopo.o $(ODIR)/lmoments.o $(ODIR)/mclin.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mcsplines.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2skel2.c $(OBJ_COMMON) $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/lseltopo.o $(ODIR)/mclin.o $(ODIR)/lmoments.o $(ODIR)/llabelextrema.o $(ODIR)/lskelcurv.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mcsplines.o $(LIBS) -o $(BDIR)/pgm2skel2

$(BDIR)/pgm2vskel:	$(CDIR)/pgm2vskel.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelcurv.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/lskelcurv.o $(ODIR)/llabelextrema.o $(ODIR)/lseltopo.o $(ODIR)/lmoments.o $(ODIR)/mclin.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mcsplines.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/pgm2vskel.c $(OBJ_COMMON) $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/lseltopo.o $(ODIR)/mclin.o $(ODIR)/lmoments.o $(ODIR)/llabelextrema.o $(ODIR)/lskelcurv.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mcsplines.o $(LIBS) -o $(BDIR)/pgm2vskel

$(BDIR)/ptcurve:	$(CDIR)/ptcurve.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptcurve.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptcurve

$(BDIR)/ptend:	$(CDIR)/ptend.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptend.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptend

$(BDIR)/ptextensible:	$(CDIR)/ptextensible.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/mcfah.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptextensible.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptextensible

$(BDIR)/ptisolated:	$(CDIR)/ptisolated.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptisolated.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptisolated

$(BDIR)/ptinterior:	$(CDIR)/ptinterior.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptinterior.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptinterior

$(BDIR)/ptjunction:	$(CDIR)/ptjunction.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptjunction.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptjunction

$(BDIR)/ptmultiple:	$(CDIR)/ptmultiple.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptmultiple.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptmultiple

$(BDIR)/ptseparating:	$(CDIR)/ptseparating.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptseparating.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptseparating

$(BDIR)/ptseparatinggray:	$(CDIR)/ptseparatinggray.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptseparatinggray.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptseparatinggray

$(BDIR)/pthseparatinggray:	$(CDIR)/pthseparatinggray.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pthseparatinggray.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/pthseparatinggray

$(BDIR)/ptselectgray:	$(CDIR)/ptselectgray.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptselectgray.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptselectgray

$(BDIR)/ptsimple:	$(CDIR)/ptsimple.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptsimple.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptsimple

$(BDIR)/ptthickend:	$(CDIR)/ptthickend.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptthickend.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptthickend

$(BDIR)/ptthickisthmus:	$(CDIR)/ptthickisthmus.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lskelpar3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lskelpar3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptthickisthmus.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptthickisthmus

$(BDIR)/ptWsimple:	$(CDIR)/ptWsimple.c $(IDIR)/mcimage.h $(IDIR)/llabelextrema.h $(OBJ_COMMON) $(ODIR)/llabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/larith.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ptWsimple.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ptWsimple

$(BDIR)/seltopo:	$(CDIR)/seltopo.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/seltopo.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/seltopo

$(BDIR)/simplepair:	$(CDIR)/simplepair.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/lseltopo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/simplepair.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/simplepair

$(BDIR)/skelsmoothing:	$(CDIR)/skelsmoothing.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelcurv.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/lskelcurv.o $(ODIR)/llabelextrema.o $(ODIR)/lseltopo.o $(ODIR)/lmoments.o $(ODIR)/mclin.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mcsplines.o $(ODIR)/lbresen.o $(ODIR)/ldraw.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelsmoothing.c $(OBJ_COMMON) $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/lseltopo.o $(ODIR)/mclin.o $(ODIR)/lmoments.o $(ODIR)/llabelextrema.o $(ODIR)/lskelcurv.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mcsplines.o $(LIBS) -o $(BDIR)/skelsmoothing

$(BDIR)/squel:	$(CDIR)/squel.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lsquel.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/squel.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/squel

$(BDIR)/skel2graph:	$(CDIR)/skel2graph.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mccodimage.h $(ODIR)/mccodimage.o $(IDIR)/lskelcurv.h $(ODIR)/lskelcurv.o $(IDIR)/lskel2graph.h $(ODIR)/lskel2graph.o $(IDIR)/lseltopo.h $(ODIR)/lseltopo.o $(IDIR)/mcgraphe.h $(ODIR)/mcgraphe.o $(OBJ_COMMON)
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel2graph.c $(OBJ_COMMON) $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel2graph

$(BDIR)/skel2pgm:	$(CDIR)/skel2pgm.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelcurv.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/lskelcurv.o $(ODIR)/llabelextrema.o $(ODIR)/lseltopo.o $(ODIR)/lmoments.o $(ODIR)/mclin.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mcsplines.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel2pgm.c $(OBJ_COMMON) $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/lseltopo.o $(ODIR)/lmoments.o $(ODIR)/mclin.o $(ODIR)/llabelextrema.o $(ODIR)/lskelcurv.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mcsplines.o $(LIBS) -o $(BDIR)/skel2pgm

$(BDIR)/skel2pov:	$(CDIR)/skel2pov.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelcurv.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/lskelcurv.o $(ODIR)/llabelextrema.o $(ODIR)/lseltopo.o $(ODIR)/lmoments.o $(ODIR)/mclin.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mcsplines.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel2pov.c $(OBJ_COMMON) $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/lseltopo.o $(ODIR)/lmoments.o $(ODIR)/mclin.o $(ODIR)/llabelextrema.o $(ODIR)/lskelcurv.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mcsplines.o $(LIBS) -o $(BDIR)/skel2pov

$(BDIR)/skel_ACK3:	$(CDIR)/skel_ACK3.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar3d.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(ODIR)/mcrlifo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/lskelpar3d.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_ACK3.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_ACK3

$(BDIR)/skel_ACK3a:	$(CDIR)/skel_ACK3a.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskel_ACK3a.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/lskel_ACK3a.o $(ODIR)/mcindic.o $(ODIR)/mcrlifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_ACK3a.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_ACK3a

$(BDIR)/skel_ACK3_pers:	$(CDIR)/skel_ACK3_pers.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar3d.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/lskelpar3d.o $(ODIR)/mcindic.o $(ODIR)/mcrlifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_ACK3_pers.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_ACK3_pers

$(BDIR)/skel_AK2:	$(CDIR)/skel_AK2.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo.o $(ODIR)/lskelpar.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_AK2.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_AK2

$(BDIR)/skel_AMK:	$(CDIR)/skel_AMK.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo.o $(ODIR)/lskelpar.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_AMK.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_AMK

$(BDIR)/skel_ASK3p:	$(CDIR)/skel_ASK3p.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo.o $(ODIR)/lskelpar.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_ASK3p.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_ASK3p

$(BDIR)/skel_CK3a:	$(CDIR)/skel_CK3a.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar3d.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/lskelpar3d.o $(ODIR)/mcindic.o $(ODIR)/mcrlifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_CK3a.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_CK3a

$(BDIR)/skel_CK3p:	$(CDIR)/skel_CK3p.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar3d.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/lskelpar3d.o $(ODIR)/mcindic.o $(ODIR)/mcrlifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_CK3p.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_CK3p

$(BDIR)/skel_CK3_pers:	$(CDIR)/skel_CK3_pers.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar3d.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/lskelpar3d.o $(ODIR)/mcindic.o $(ODIR)/mcrlifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_CK3_pers.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_CK3_pers

$(BDIR)/skel_CKG:	$(CDIR)/skel_CKG.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mckhalimsky3d.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_CKG.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_CKG

$(BDIR)/skel_CK2_pers:	$(CDIR)/skel_CK2_pers.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mckhalimsky3d.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_CK2_pers.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_CK2_pers

$(BDIR)/skel_CKG_map:	$(CDIR)/skel_CKG_map.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mckhalimsky3d.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_CKG_map.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_CKG_map

$(BDIR)/skel_CKSC2:	$(CDIR)/skel_CKSC2.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskeletons.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/lskeletons.o $(ODIR)/mcindic.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_CKSC2.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_CKSC2

$(BDIR)/skel_CKSS3:	$(CDIR)/skel_CKSS3.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskeletons.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/lskeletons.o $(ODIR)/mcindic.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_CKSS3.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_CKSS3

$(BDIR)/skel_EK3:	$(CDIR)/skel_EK3.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar3d.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/lskelpar3d.o $(ODIR)/mcindic.o $(ODIR)/mcrlifo.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_EK3.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_EK3

$(BDIR)/skel_MK2:	$(CDIR)/skel_MK2.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo.o $(ODIR)/lskelpar.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_MK2.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_MK2

$(BDIR)/skel_MK3:	$(CDIR)/skel_MK3.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar3d.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/lskelpar3d.o $(ODIR)/mcindic.o $(ODIR)/mcrlifo.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_MK3.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_MK3

$(BDIR)/skel_NK2:	$(CDIR)/skel_NK2.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo.o $(ODIR)/lskelpar.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_NK2.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_NK2

$(BDIR)/skel_NK2_pers:	$(CDIR)/skel_NK2_pers.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mckhalimsky3d.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_NK2_pers.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_NK2_pers

$(BDIR)/skel_PSG:	$(CDIR)/skel_PSG.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mckhalimsky3d.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_PSG.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_PSG

$(BDIR)/skel_SCK3_pers:	$(CDIR)/skel_SCK3_pers.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar3d.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/lskelpar3d.o $(ODIR)/mcindic.o $(ODIR)/mcrlifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_SCK3_pers.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_SCK3_pers

$(BDIR)/skel_SK3_pers:	$(CDIR)/skel_SK3_pers.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar3d.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/lskelpar3d.o $(ODIR)/mcindic.o $(ODIR)/mcrlifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_SK3_pers.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_SK3_pers

$(BDIR)/skel_SK3:	$(CDIR)/skel_SK3.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelpar3d.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo3d.o $(ODIR)/mctopo.o $(ODIR)/lskelpar3d.o $(ODIR)/mcindic.o $(ODIR)/mcrlifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skel_SK3.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skel_SK3

$(BDIR)/skelend:	$(CDIR)/skelend.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfifo.h $(IDIR)/lsquelbin.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcfifo.o $(ODIR)/mclifo.o $(ODIR)/mcgeo.o $(ODIR)/mcrbt.o $(ODIR)/lsquelbin.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelend.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skelend

$(BDIR)/skelendlab:	$(CDIR)/skelendlab.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfifo.h $(IDIR)/lsquelbin.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcfifo.o $(ODIR)/mclifo.o $(ODIR)/mcgeo.o $(ODIR)/mcrbt.o $(ODIR)/lsquelbin.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelendlab.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skelendlab

$(BDIR)/skelcurv:	$(CDIR)/skelcurv.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcfifo.o $(ODIR)/mclifo.o $(ODIR)/mcliste.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelcurv.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skelcurv

$(BDIR)/skelcurv2:	$(CDIR)/skelcurv2.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcfifo.o $(ODIR)/mclifo.o $(ODIR)/mcliste.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelcurv2.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skelcurv2

$(BDIR)/skelcurvend:	$(CDIR)/skelcurvend.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcfifo.o $(ODIR)/mclifo.o $(ODIR)/mcliste.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelcurvend.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skelcurvend

$(BDIR)/skelcurvfilter:	$(CDIR)/skelcurvfilter.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcfifo.o $(ODIR)/mclifo.o $(ODIR)/mcliste.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelcurvfilter.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skelcurvfilter

$(BDIR)/skeleton:	$(CDIR)/skeleton.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mckhalimsky3d.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skeleton.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skeleton

$(BDIR)/skeletonlab:	$(CDIR)/skeletonlab.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mckhalimsky3d.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skeletonlab.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skeletonlab

$(BDIR)/skeleucl:	$(CDIR)/skeleucl.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(IDIR)/mcgeo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skeleucl.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skeleucl

$(BDIR)/skelfindelbows:	$(CDIR)/skelfindelbows.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelcurv.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/lskelcurv.o $(ODIR)/llabelextrema.o $(ODIR)/lseltopo.o $(ODIR)/lmoments.o $(ODIR)/mclin.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mcsplines.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelfindelbows.c $(OBJ_COMMON) $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/lseltopo.o $(ODIR)/lmoments.o $(ODIR)/mclin.o $(ODIR)/llabelextrema.o $(ODIR)/lskelcurv.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mcsplines.o $(LIBS) -o $(BDIR)/skelfindelbows

$(BDIR)/skelpar:	$(CDIR)/skelpar.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/lskelpar.h $(IDIR)/ldist.h $(IDIR)/mcgeo.h $(IDIR)/lmedialaxis.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ltopotypes.o $(ODIR)/lskelpar.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelpar.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skelpar

$(BDIR)/skelpar3d:	$(CDIR)/skelpar3d.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/lskelpar3d.h $(IDIR)/ldist.h $(IDIR)/mcgeo.h $(IDIR)/lmedialaxis.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ltopotypes.o $(ODIR)/lskelpar3d.o $(ODIR)/mcrlifo.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelpar3d.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skelpar3d

$(BDIR)/skelpar3d_others:	$(CDIR)/skelpar3d_others.cxx $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mcindic.h $(IDIR)/lskelpar3d_others.h $(OBJ_COMMON) $(ODIR)/mctopo3d.o $(ODIR)/mccodimage.o $(ODIR)/lskeletons.o $(ODIR)/lskelpar3d.o $(ODIR)/lskel_ACK3a.o $(ODIR)/lskelpar3d_others.o $(ODIR)/DirectionalSkeletonizer.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelpar3d_others.cxx $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skelpar3d_others

$(BDIR)/skelpruning:	$(CDIR)/skelpruning.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mccodimage.h $(ODIR)/mccodimage.o $(IDIR)/lskelcurv.h $(ODIR)/lskelcurv.o $(IDIR)/lseltopo.h $(ODIR)/lseltopo.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelpruning.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skelpruning

$(BDIR)/skelsurf:	$(CDIR)/skelsurf.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(IDIR)/mcgeo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelsurf.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skelsurf

$(BDIR)/skelvertex:	$(CDIR)/skelvertex.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lskelcurv.h $(IDIR)/lseltopo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/lskelcurv.o $(ODIR)/llabelextrema.o $(ODIR)/lseltopo.o $(ODIR)/lmoments.o $(ODIR)/mclin.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mcsplines.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/skelvertex.c $(OBJ_COMMON) $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcskelcurv.o $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/lseltopo.o $(ODIR)/lmoments.o $(ODIR)/mclin.o $(ODIR)/llabelextrema.o $(ODIR)/lskelcurv.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/lbdigitalline.o $(ODIR)/ltangents.o $(ODIR)/mcsplines.o $(LIBS) -o $(BDIR)/skelvertex

$(BDIR)/bisector:	$(CDIR)/bisector.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/mcgeo.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o $(ODIR)/ldist.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ltopotypes.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/bisector.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/bisector

$(BDIR)/bisector_talbot:	$(CDIR)/bisector_talbot.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/mcgeo.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o $(ODIR)/ldist.o $(ODIR)/lballincl.o $(ODIR)/lmedialaxis.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/ltopotypes.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/bisector_talbot.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/bisector_talbot

$(BDIR)/squelval:	$(CDIR)/squelval.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcliste.h $(IDIR)/lbresen.h $(IDIR)/lsquelval.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/lsquelval.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/squelval.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/squelval

$(BDIR)/surfacerestoration:	$(CDIR)/surfacerestoration.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo3d.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/mcfah.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/surfacerestoration.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/surfacerestoration

$(BDIR)/topotypes:	$(CDIR)/topotypes.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/ltopotypes.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/ltopotypes.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/topotypes.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/topotypes

$(BDIR)/t26pp:	$(CDIR)/t26pp.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/ltopotypes.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/ltopotypes.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/t26pp.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/t26pp

$(BDIR)/t4pp:	$(CDIR)/t4pp.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/ltopotypes.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/ltopotypes.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/t4pp.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/t4pp

$(BDIR)/t8pp:	$(CDIR)/t8pp.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/ltopotypes.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/ltopotypes.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/t8pp.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/t8pp

$(BDIR)/taf:	$(CDIR)/taf.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/lfiltrestopo.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(OBJMCFAH) $(ODIR)/mcindic.o $(ODIR)/lfiltrestopo.o $(ODIR)/lhtkern.o $(ODIR)/lhtkern3d.o $(ODIR)/llabelextrema.o $(ODIR)/llambdakern.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/taf.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/taf

$(BDIR)/tlf:	$(CDIR)/tlf.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/lfiltrestopo.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(OBJMCFAH) $(ODIR)/mcindic.o $(ODIR)/lfiltrestopo.o $(ODIR)/lhtkern.o $(ODIR)/lhtkern3d.o $(ODIR)/llabelextrema.o $(ODIR)/llambdakern.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/tlf.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/tlf

$(BDIR)/tuf:	$(CDIR)/tuf.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/lfiltrestopo.h $(IDIR)/lhtkern.h $(IDIR)/lhtkern3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(OBJMCFAH) $(ODIR)/mcindic.o $(ODIR)/lfiltrestopo.o $(ODIR)/lhtkern.o $(ODIR)/lhtkern3d.o $(ODIR)/llabelextrema.o $(ODIR)/llambdakern.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/tuf.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/tuf

$(BDIR)/toposhrink:	$(CDIR)/toposhrink.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/toposhrink.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/toposhrink

$(BDIR)/toposhrinkgray:	$(CDIR)/toposhrinkgray.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcfah.h $(IDIR)/mcindic.h $(IDIR)/lskeletons.h $(IDIR)/ldist.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(OBJMCFAH) $(ODIR)/mclifo.o $(ODIR)/mcfifo.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcindic.o $(ODIR)/mcrbt.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcrlifo.o $(ODIR)/lskeletons.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/toposhrinkgray.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/toposhrinkgray

# *********************************
# ORDRES
# *********************************

$(BDIR)/2dborder:	$(CDIR)/2dborder.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l2dkhalimsky.h $(ODIR)/l2dkhalimsky.o $(IDIR)/mckhalimsky2d.h $(ODIR)/mckhalimsky2d.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dborder.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dborder

$(BDIR)/2dcollapse:	$(CDIR)/2dcollapse.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcindic.h $(ODIR)/mcindic.o $(IDIR)/ldist.h $(ODIR)/ldist.o $(OBJ_COMMON) $(IDIR)/l2dcollapse.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dcollapse.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dcollapse

$(BDIR)/2dcolor:	$(CDIR)/2dcolor.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l2dkhalimsky.h $(ODIR)/l2dkhalimsky.o $(IDIR)/mckhalimsky2d.h $(ODIR)/mckhalimsky2d.o $(ODIR)/mclifo.o $(ODIR)/mcindic.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dcolor.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dcolor

$(BDIR)/2dflowskeleton:	$(CDIR)/2dflowskeleton.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcindic.h $(ODIR)/mcindic.o $(IDIR)/ldist.h $(ODIR)/ldist.o $(IDIR)/lmedialaxis.h $(ODIR)/lmedialaxis.o $(IDIR)/l2dkhalimsky.h $(ODIR)/l2dkhalimsky.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dflowskeleton.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dflowskeleton

$(BDIR)/2dinvariants:	$(CDIR)/2dinvariants.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l2dkhalimsky.h $(ODIR)/l2dkhalimsky.o $(IDIR)/mckhalimsky2d.h $(ODIR)/mckhalimsky2d.o $(ODIR)/mclifo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dinvariants.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dinvariants

$(BDIR)/2dkhalimskize:	$(CDIR)/2dkhalimskize.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l2dkhalimsky.h $(ODIR)/l2dkhalimsky.o $(IDIR)/mckhalimsky2d.h $(ODIR)/mckhalimsky2d.o $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(ODIR)/bdd1.alphacube.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dkhalimskize.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dkhalimskize

$(BDIR)/2dlabel:	$(CDIR)/2dlabel.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l2dkhalimsky.h $(ODIR)/l2dkhalimsky.o $(IDIR)/mckhalimsky2d.h $(ODIR)/mckhalimsky2d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/mcindic.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dlabel.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dlabel

$(BDIR)/2dmakecomplex:	$(CDIR)/2dmakecomplex.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l2dkhalimsky.h $(ODIR)/l2dkhalimsky.o $(IDIR)/mckhalimsky2d.h $(ODIR)/mckhalimsky2d.o $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(ODIR)/bdd1.alphacube.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dmakecomplex.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dmakecomplex

$(BDIR)/2dmakesimplicialcomplex:	$(CDIR)/2dmakesimplicialcomplex.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l2dkhalimsky.h $(ODIR)/l2dkhalimsky.o $(IDIR)/mckhalimsky2d.h $(ODIR)/mckhalimsky2d.o $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(ODIR)/bdd1.alphacube.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dmakesimplicialcomplex.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dmakesimplicialcomplex

$(BDIR)/2dpardircollapse:	$(CDIR)/2dpardircollapse.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcindic.h $(ODIR)/mcindic.o $(IDIR)/ldist.h $(ODIR)/ldist.o $(OBJ_COMMON) $(IDIR)/l2dcollapse.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dpardircollapse.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dpardircollapse

$(BDIR)/2dseltopo:	$(CDIR)/2dseltopo.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l2dkhalimsky.h $(ODIR)/l2dkhalimsky.o $(IDIR)/mckhalimsky2d.h $(ODIR)/mckhalimsky2d.o $(ODIR)/mclifo.o $(ODIR)/mcindic.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dseltopo.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dseltopo

$(BDIR)/2dskelck:	$(CDIR)/2dskelck.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcskel2d.h $(ODIR)/mcskel2d.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dskelck.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dskelck

$(BDIR)/2dthin:	$(CDIR)/2dthin.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l2dkhalimsky.h $(ODIR)/l2dkhalimsky.o $(IDIR)/mckhalimsky2d.h $(ODIR)/mckhalimsky2d.o $(ODIR)/mclifo.o $(ODIR)/mcindic.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dthin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dthin

$(BDIR)/2dtopoflow:	$(CDIR)/2dtopoflow.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcindic.h $(ODIR)/mcindic.o $(IDIR)/ldist.h $(ODIR)/ldist.o $(OBJ_COMMON) $(IDIR)/l2dcollapse.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/2dtopoflow.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/2dtopoflow

$(BDIR)/3dalpha:	$(CDIR)/3dalpha.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dalpha.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dalpha

$(BDIR)/3dbeta:	$(CDIR)/3dbeta.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dbeta.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dbeta

$(BDIR)/3dborder:	$(CDIR)/3dborder.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dborder.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dborder

$(BDIR)/3dcollapse:	$(CDIR)/3dcollapse.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcindic.h $(ODIR)/mcindic.o $(IDIR)/ldist.h $(ODIR)/ldist.o $(OBJ_COMMON) $(IDIR)/l3dcollapse.h $(ODIR)/l3dcollapse.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dcollapse.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dcollapse

$(BDIR)/3dpardircollapse:	$(CDIR)/3dpardircollapse.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcindic.h $(ODIR)/mcindic.o $(IDIR)/ldist.h $(ODIR)/ldist.o $(OBJ_COMMON) $(IDIR)/l3dcollapse.h $(ODIR)/l3dcollapse.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dpardircollapse.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dpardircollapse

$(BDIR)/3dsurfacecollapse:	$(CDIR)/3dsurfacecollapse.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcindic.h $(ODIR)/mcindic.o $(IDIR)/ldist.h $(ODIR)/ldist.o $(OBJ_COMMON) $(IDIR)/l3dcollapse.h $(ODIR)/l3dcollapse.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dsurfacecollapse.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dsurfacecollapse

$(BDIR)/3dcolor:	$(CDIR)/3dcolor.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dcolor.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dcolor

$(BDIR)/3dflowskeleton:	$(CDIR)/3dflowskeleton.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcindic.h $(ODIR)/mcindic.o $(IDIR)/ldist.h $(ODIR)/ldist.o $(IDIR)/lmedialaxis.h $(ODIR)/lmedialaxis.o $(IDIR)/l3dcollapse.h $(ODIR)/l3dcollapse.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dflowskeleton.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dflowskeleton

$(BDIR)/3dinvariants:	$(CDIR)/3dinvariants.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dinvariants.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dinvariants

$(BDIR)/3disthmus:	$(CDIR)/3disthmus.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3disthmus.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3disthmus

$(BDIR)/3dkhalimskize:	$(CDIR)/3dkhalimskize.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dkhalimskize.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dkhalimskize

$(BDIR)/3dlabel:	$(CDIR)/3dlabel.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dlabel.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dlabel

$(BDIR)/3dmakecomplex:	$(CDIR)/3dmakecomplex.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dmakecomplex.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dmakecomplex

$(BDIR)/3dmakepolygons:	$(CDIR)/3dmakepolygons.c $(IDIR)/mcimage.h $(IDIR)/mcliste.h $(IDIR)/l3dkhalimsky.h $(IDIR)/mcpolygons.h $(OBJ_COMMON) $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(IDIR)/mcgeo.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/mcliste.o $(ODIR)/mcindic.o $(ODIR)/mcgeo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/mcrbt.o $(ODIR)/mcpolygons.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dmakepolygons.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dmakepolygons

$(BDIR)/3dplane:	$(CDIR)/3dplane.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dplane.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dplane

$(BDIR)/3drecons:	$(CDIR)/3drecons.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3drecons.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3drecons

$(BDIR)/3dseltopo:	$(CDIR)/3dseltopo.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dseltopo.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dseltopo

$(BDIR)/3dskelck:	$(CDIR)/3dskelck.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/mcskel3d.h $(ODIR)/mcskel3d.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dskelck.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dskelck

$(BDIR)/3dskelsurf:	$(CDIR)/3dskelsurf.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dskelsurf.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dskelsurf

$(BDIR)/3dsphere:	$(CDIR)/3dsphere.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dsphere.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dsphere

$(BDIR)/3dthin:	$(CDIR)/3dthin.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/l3dkhalimsky.h $(ODIR)/l3dkhalimsky.o $(IDIR)/mckhalimsky3d.h $(ODIR)/mckhalimsky3d.o $(ODIR)/mclifo.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/3dthin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/3dthin

# *********************************
# DRAW
# *********************************

$(BDIR)/animscenes:	$(CDIR)/animscenes.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/mcgeo.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o $(ODIR)/mcgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/animscenes.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/animscenes

$(BDIR)/drawball:	$(CDIR)/drawball.c $(IDIR)/mcimage.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawball.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawball

$(BDIR)/drawcurve:	$(CDIR)/drawcurve.c $(IDIR)/mcimage.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawcurve.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawcurve

$(BDIR)/drawcylinder:	$(CDIR)/drawcylinder.c $(IDIR)/mcimage.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawcylinder.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawcylinder

$(BDIR)/drawfield:	$(CDIR)/drawfield.c $(IDIR)/mcimage.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawfield.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawfield

$(BDIR)/drawfieldlists:	$(CDIR)/drawfieldlists.c $(IDIR)/mcimage.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawfieldlists.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawfieldlists

$(BDIR)/drawgraph:	$(CDIR)/drawgraph.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/lbresen.h $(ODIR)/lbresen.o $(IDIR)/ldraw.h $(ODIR)/ldraw.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawgraph.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawgraph

$(BDIR)/drawline:	$(CDIR)/drawline.c $(IDIR)/mcimage.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawline.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawline

$(BDIR)/drawtriangle:	$(CDIR)/drawtriangle.c $(IDIR)/mcimage.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawtriangle.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawtriangle

$(BDIR)/drawquadrangle:	$(CDIR)/drawquadrangle.c $(IDIR)/mcimage.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawquadrangle.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawquadrangle

$(BDIR)/drawlines:	$(CDIR)/drawlines.c $(IDIR)/mcimage.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawlines.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawlines

$(BDIR)/drawellipse:	$(CDIR)/drawellipse.c $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/lborder.h $(IDIR)/lellipseincl.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclin.o $(ODIR)/lbresen.o $(ODIR)/lborder.o $(ODIR)/lellipseincl.o $(ODIR)/ldraw.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawellipse.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawellipse

$(BDIR)/drawrect:	$(CDIR)/drawrect.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawrect.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawrect

$(BDIR)/drawscene:	$(CDIR)/drawscene.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawscene.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawscene

$(BDIR)/drawspline:	$(CDIR)/drawspline.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawspline.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawspline

$(BDIR)/drawsplineorient:	$(CDIR)/drawsplineorient.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawsplineorient.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawsplineorient

$(BDIR)/drawsplinesorient:	$(CDIR)/drawsplinesorient.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawsplinesorient.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawsplinesorient

$(BDIR)/drawsplinesorient2:	$(CDIR)/drawsplinesorient2.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawsplinesorient2.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawsplinesorient2

$(BDIR)/drawsplines:	$(CDIR)/drawsplines.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawsplines.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawsplines

$(BDIR)/drawtorus:	$(CDIR)/drawtorus.c $(IDIR)/mcimage.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawtorus.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawtorus

$(BDIR)/drawtriangulation:	$(CDIR)/drawtriangulation.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mcgeo.h $(IDIR)/lvoronoi.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcgeo.o $(ODIR)/mcliste.o $(ODIR)/lvoronoi.o $(ODIR)/lbresen.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/drawtriangulation.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/drawtriangulation

$(BDIR)/genball:	$(CDIR)/genball.c $(IDIR)/mcimage.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/genball.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/genball

$(BDIR)/lengthspline:	$(CDIR)/lengthspline.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lengthspline.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lengthspline

$(BDIR)/lengthsplines:	$(CDIR)/lengthsplines.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lengthsplines.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lengthsplines

$(BDIR)/line:	$(CDIR)/line.c $(IDIR)/ldraw.h $(IDIR)/lbresen.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/ldraw.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/line.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/line

$(BDIR)/curve2spline:	$(CDIR)/curve2spline.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/curve2spline.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/curve2spline

$(BDIR)/points2spline:	$(CDIR)/points2spline.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/points2spline.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/points2spline

$(BDIR)/samplespline:	$(CDIR)/samplespline.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/samplespline.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/samplespline

$(BDIR)/worms:	$(CDIR)/worms.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/mcprobas.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(IDIR)/lrotations.h $(IDIR)/ldilateros3d.h $(IDIR)/lcrop.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mclifo.o $(ODIR)/mcgeo.o $(ODIR)/ldist.o $(ODIR)/mcsplines.o $(ODIR)/mcprobas.o $(ODIR)/lrotations.o $(ODIR)/ldilateros3d.o $(ODIR)/lcrop.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/worms.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/worms

# *********************************
# GEO
# *********************************

$(BDIR)/affine:	$(CDIR)/affine.c $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/mcgeo.h $(IDIR)/lrotations.h $(OBJ_COMMON) $(ODIR)/mclin.o $(ODIR)/mcgeo.o $(ODIR)/lrotations.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/affine.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/affine

$(BDIR)/attribute:	$(CDIR)/attribute.c $(IDIR)/lattribute.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo.o $(ODIR)/mclin.o $(ODIR)/lattribute.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/attribute.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/attribute

$(BDIR)/autocrop:	$(CDIR)/autocrop.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/lcrop.h $(ODIR)/lcrop.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/autocrop.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/autocrop

$(BDIR)/barycentre:	$(CDIR)/barycentre.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/llabelextrema.h $(IDIR)/lbarycentre.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/lbarycentre.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/barycentre.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/barycentre

$(BDIR)/barycentrelab:	$(CDIR)/barycentrelab.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/llabelextrema.h $(IDIR)/lbarycentre.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/lbarycentre.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/barycentrelab.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/barycentrelab

$(BDIR)/boxmin:	$(CDIR)/boxmin.c $(IDIR)/mcimage.h $(IDIR)/lgeo.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mclin.o $(ODIR)/mcgeo.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/llabelextrema.o $(ODIR)/lgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/boxmin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/boxmin

$(BDIR)/cadre:	$(CDIR)/cadre.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/cadre.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/cadre

$(BDIR)/convexhull:	$(CDIR)/convexhull.c $(IDIR)/lconvexhull.h $(IDIR)/mcimage.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lconvexhull.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/lbresen.o $(ODIR)/mcfifo.o $(ODIR)/mcindic.o $(ODIR)/lgeodesic.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/convexhull.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/convexhull

$(BDIR)/crop:	$(CDIR)/crop.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/lcrop.h $(ODIR)/lcrop.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/crop.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/crop

$(BDIR)/cropondisk: $(CDIR)/cropondisk.c $(ODIR)/libcrop.o $(IDIR)/mccodimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) -D_LARGEFILE64_SOURCE=1 -D_FILE_OFFSET_BITS=64 $< $(ODIR)/libcrop.o $(OBJ_COMMON) $(LIBS) -o  $(BDIR)/cropondisk

$(BDIR)/shrinkondisk: $(CDIR)/shrinkondisk.c $(ODIR)/libcrop.o $(IDIR)/mccodimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) -D_LARGEFILE64_SOURCE=1 -D_FILE_OFFSET_BITS=64 $< $(ODIR)/libcrop.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/shrinkondisk

$(BDIR)/curvatures:	$(CDIR)/curvatures.c $(IDIR)/mcimage.h $(IDIR)/mcsplines.h $(IDIR)/lbresen.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/ldraw.o $(ODIR)/mcliste.o $(ODIR)/mclin.o $(ODIR)/mcsplines.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/curvatures.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/curvatures

$(BDIR)/curvelength:	$(CDIR)/curvelength.cxx $(IDIR)/ltangents.h $(OBJ_COMMON) $(ODIR)/lbdigitalline.o $(ODIR)/mclin.o $(ODIR)/ltangents.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/curvelength.cxx $(OBJ_COMMON) $(ODIR)/lbdigitalline.o $(ODIR)/mclin.o $(ODIR)/ltangents.o $(LIBS) -o $(BDIR)/curvelength

$(BDIR)/curvetangents:	$(CDIR)/curvetangents.cxx $(IDIR)/ltangents.h $(OBJ_COMMON) $(ODIR)/lbdigitalline.o $(ODIR)/mclin.o $(ODIR)/ltangents.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/curvetangents.cxx $(OBJ_COMMON) $(ODIR)/lbdigitalline.o $(ODIR)/mclin.o $(ODIR)/ltangents.o $(LIBS) -o $(BDIR)/curvetangents

$(BDIR)/curvenormalplanes:	$(CDIR)/curvenormalplanes.cxx $(IDIR)/ltangents.h $(OBJ_COMMON) $(ODIR)/lbdigitalline.o $(ODIR)/mclin.o $(ODIR)/ltangents.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/curvenormalplanes.cxx $(OBJ_COMMON) $(ODIR)/lbdigitalline.o $(ODIR)/mclin.o $(ODIR)/ltangents.o $(LIBS) -o $(BDIR)/curvenormalplanes

$(BDIR)/curve2segments:	$(CDIR)/curve2segments.cxx $(IDIR)/ltangents.h $(OBJ_COMMON) $(ODIR)/lbdigitalline.o $(ODIR)/mclin.o $(ODIR)/ltangents.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/curve2segments.cxx $(OBJ_COMMON) $(ODIR)/lbdigitalline.o $(ODIR)/mclin.o $(ODIR)/ltangents.o $(LIBS) -o $(BDIR)/curve2segments

$(BDIR)/curvesections:	$(CDIR)/curvesections.c $(IDIR)/lgeo.h $(IDIR)/mcsplines.h $(IDIR)/mclin.h $(OBJ_COMMON) $(ODIR)/lgeo.o $(ODIR)/mcsplines.o $(ODIR)/mclin.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/curvesections.c $(OBJ_COMMON) $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/curvesections

$(BDIR)/deframe:	$(CDIR)/deframe.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/lcrop.h $(ODIR)/lcrop.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/deframe.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/deframe

$(BDIR)/delaunay:	$(CDIR)/delaunay.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mcgeo.h $(IDIR)/lvoronoi.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcgeo.o $(ODIR)/mcliste.o $(ODIR)/lvoronoi.o $(ODIR)/lbresen.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/delaunay.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/delaunay

$(BDIR)/delaunaymask:	$(CDIR)/delaunaymask.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mcgeo.h $(IDIR)/lvoronoi.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcgeo.o $(ODIR)/mcliste.o $(ODIR)/lvoronoi.o $(ODIR)/lbresen.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/delaunaymask.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/delaunaymask

$(BDIR)/detectcercles:	$(CDIR)/detectcercles.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o $(ODIR)/ldetectcercles.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/detectcercles.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/detectcercles

$(BDIR)/directions:	$(CDIR)/directions.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldirections.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/ldirections.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/directions.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/directions

$(BDIR)/eden:	$(CDIR)/eden.cxx $(IDIR)/leden.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/leden.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mcset.o $(ODIR)/mcprobas.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/eden.cxx $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/mcset.o $(ODIR)/mcprobas.o $(ODIR)/leden.o $(LIBS) -o $(BDIR)/eden

$(BDIR)/ellipsefind:	$(CDIR)/ellipsefind.c $(IDIR)/lmoments.h $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mclin.o $(ODIR)/lmoments.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ellipsefind.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ellipsefind

$(BDIR)/ellipsefit:	$(CDIR)/ellipsefit.c $(IDIR)/lellipsefit.h $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mclin.o $(ODIR)/lellipsefit.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ellipsefit.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ellipsefit

$(BDIR)/ellipseparams:	$(CDIR)/ellipseparams.c $(IDIR)/lellipsefit.h $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mclin.o $(ODIR)/lellipsefit.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ellipseparams.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ellipseparams

$(BDIR)/ellipseincl:	$(CDIR)/ellipseincl.c $(IDIR)/lellipseincl.h $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/lborder.h $(IDIR)/ldraw.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclin.o $(ODIR)/lellipseincl.o $(ODIR)/lbresen.o $(ODIR)/lborder.o $(ODIR)/ldraw.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ellipseincl.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ellipseincl

$(BDIR)/ellipticite:	$(CDIR)/ellipticite.c $(IDIR)/lgeo.h $(IDIR)/mcgeo.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/llabelextrema.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o $(ODIR)/mcgeo.o $(ODIR)/mctopo.o $(ODIR)/mclin.o $(ODIR)/lgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ellipticite.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ellipticite

$(BDIR)/encadre:	$(CDIR)/encadre.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/lcrop.h $(ODIR)/lcrop.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/encadre.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/encadre

$(BDIR)/enframe:	$(CDIR)/enframe.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/lcrop.h $(ODIR)/lcrop.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/enframe.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/enframe

$(BDIR)/enframe3d:	$(CDIR)/enframe3d.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/lcrop.h $(ODIR)/lcrop.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/enframe3d.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/enframe3d

$(BDIR)/expandframe:	$(CDIR)/expandframe.c $(IDIR)/mcimage.h $(IDIR)/lcrop.h $(OBJ_COMMON) $(ODIR)/lcrop.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/expandframe.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/expandframe

$(BDIR)/extractline:	$(CDIR)/extractline.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/lbresen.h $(ODIR)/lbresen.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/extractline.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/extractline

$(BDIR)/extractplane:	$(CDIR)/extractplane.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/extractplane.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/extractplane

$(BDIR)/frame:	$(CDIR)/frame.c $(IDIR)/mcimage.h $(IDIR)/lcrop.h $(OBJ_COMMON) $(ODIR)/lcrop.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/frame.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/frame

$(BDIR)/fitcircle:	$(CDIR)/fitcircle.c $(IDIR)/lgeo.h $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclin.o $(ODIR)/mcgeo.o $(ODIR)/mctopo.o $(ODIR)/lgeo.o $(ODIR)/lbresen.o $(ODIR)/mclifo.o $(ODIR)/mcliste.o $(ODIR)/llabelextrema.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/fitcircle.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/fitcircle

$(BDIR)/gencurv:	$(CDIR)/gencurv.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/gencurv.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/gencurv

$(BDIR)/genplane:	$(CDIR)/genplane.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/genplane.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/genplane

$(BDIR)/houghcercles:	$(CDIR)/houghcercles.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o $(ODIR)/lhoughcercles.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/houghcercles.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/houghcercles

$(BDIR)/identifyline:	$(CDIR)/identifyline.c $(IDIR)/mcgeo.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/llabelextrema.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o $(ODIR)/mcgeo.o $(ODIR)/mctopo.o $(ODIR)/mclin.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/identifyline.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/identifyline

$(BDIR)/identifyparabola2:	$(CDIR)/identifyparabola2.c $(IDIR)/mcgeo.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/llabelextrema.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o $(ODIR)/mcgeo.o $(ODIR)/mctopo.o $(ODIR)/mclin.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/identifyparabola2.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/identifyparabola2

$(BDIR)/identifyparabola3:	$(CDIR)/identifyparabola3.c $(IDIR)/mcgeo.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/llabelextrema.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o $(ODIR)/mcgeo.o $(ODIR)/mctopo.o $(ODIR)/mclin.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/identifyparabola3.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/identifyparabola3

$(BDIR)/identifyplane:	$(CDIR)/identifyplane.c $(IDIR)/mcgeo.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/llabelextrema.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o $(ODIR)/mcgeo.o $(ODIR)/mctopo.o $(ODIR)/mclin.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/identifyplane.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/identifyplane

$(BDIR)/insert:	$(CDIR)/insert.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/lcrop.h $(ODIR)/lcrop.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/insert.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/insert

$(BDIR)/isometry:	$(CDIR)/isometry.c $(IDIR)/mcimage.h $(IDIR)/lisometry.h $(OBJ_COMMON) $(ODIR)/lisometry.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/isometry.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/isometry

$(BDIR)/lenoir:	$(CDIR)/lenoir.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mcfifo.h $(IDIR)/llenoir.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcfifo.o $(ODIR)/llenoir.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/lenoir.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/lenoir

$(BDIR)/matchellipse:	$(CDIR)/matchellipse.c $(IDIR)/lgeo.h $(IDIR)/mcgeo.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/llabelextrema.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o $(ODIR)/mcgeo.o $(ODIR)/mctopo.o $(ODIR)/mclin.o $(ODIR)/lgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/matchellipse.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/matchellipse

$(BDIR)/matchrect:	$(CDIR)/matchrect.c $(IDIR)/lgeo.h $(IDIR)/mcgeo.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/llabelextrema.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o $(ODIR)/mcgeo.o $(ODIR)/mctopo.o $(ODIR)/mclin.o $(ODIR)/lgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/matchrect.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/matchrect

$(BDIR)/maxdiameter:	$(CDIR)/maxdiameter.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/lmaxdiameter.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/lmaxdiameter.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/maxdiameter.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/maxdiameter

$(BDIR)/moments:	$(CDIR)/moments.c $(IDIR)/lmoments.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/llabelextrema.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mclin.o $(ODIR)/llabelextrema.o $(ODIR)/lmoments.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/moments.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/moments

$(BDIR)/offset:	$(CDIR)/offset.c $(IDIR)/mcimage.h $(IDIR)/loffset.h $(OBJ_COMMON) $(ODIR)/loffset.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/offset.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/offset

$(BDIR)/pca:	$(CDIR)/pca.c $(IDIR)/lmoments.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mclin.h $(IDIR)/llabelextrema.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mclin.o $(ODIR)/llabelextrema.o $(ODIR)/lmoments.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/pca.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/pca

$(BDIR)/planarity:	$(CDIR)/planarity.c $(IDIR)/lattribute.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mctopo.o $(ODIR)/mclin.o $(ODIR)/lattribute.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/planarity.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/planarity

$(BDIR)/point:	$(CDIR)/point.c $(IDIR)/lpoint.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/lpoint.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/point.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/point

$(BDIR)/proj:	$(CDIR)/proj.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/proj.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/proj

$(BDIR)/proj3d:	$(CDIR)/proj3d.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/proj3d.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/proj3d

$(BDIR)/projsphere:	$(CDIR)/projsphere.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/projsphere.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/projsphere

$(BDIR)/quasishear:	$(CDIR)/quasishear.c $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/mcgeo.h $(IDIR)/lrotations.h $(OBJ_COMMON) $(ODIR)/mclin.o $(ODIR)/mcgeo.o $(ODIR)/lrotations.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/quasishear.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/quasishear

$(BDIR)/recalagerigide:	$(CDIR)/recalagerigide.c $(IDIR)/lrecalagerigide.h $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/mcpowell.h $(IDIR)/mcgeo.h $(OBJ_COMMON) $(ODIR)/mclin.o $(ODIR)/mcpowell.o $(ODIR)/lrecalagerigide.o $(ODIR)/mcgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/recalagerigide.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/recalagerigide

$(BDIR)/recalagerigide_num:	$(CDIR)/recalagerigide_num.c $(IDIR)/lrecalagerigide.h $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/mcpowell.h $(IDIR)/mcgeo.h $(OBJ_COMMON) $(ODIR)/mclin.o $(ODIR)/mcpowell.o $(ODIR)/lrecalagerigide.o $(ODIR)/lrotations.o $(ODIR)/mcgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/recalagerigide_num.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/recalagerigide_num

$(BDIR)/recalagerigide_translateplane:	$(CDIR)/recalagerigide_translateplane.c $(IDIR)/lrecalagerigide_translateplane.h $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/mcpowell.h $(IDIR)/mcgeo.h $(ODIR)/mclin.o $(ODIR)/mcpowell.o $(ODIR)/lrecalagerigide_translateplane.o $(ODIR)/mcgeo.o $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/recalagerigide_translateplane.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/recalagerigide_translateplane

$(BDIR)/recalagerigidesanszoom:	$(CDIR)/recalagerigidesanszoom.c $(IDIR)/lrecalagerigide.h $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/mcpowell.h $(IDIR)/mcgeo.h $(OBJ_COMMON) $(ODIR)/mclin.o $(ODIR)/mcpowell.o $(ODIR)/lrecalagerigide.o $(ODIR)/mcgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/recalagerigidesanszoom.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/recalagerigidesanszoom

$(BDIR)/rotate:	$(CDIR)/rotate.c $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/mcgeo.h $(IDIR)/lrotations.h $(OBJ_COMMON) $(ODIR)/mclin.o $(ODIR)/mcgeo.o $(ODIR)/lrotations.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/rotate.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/rotate

$(BDIR)/rotate3dbin:	$(CDIR)/rotate3dbin.c $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/mcgeo.h $(IDIR)/lrotations.h $(OBJ_COMMON) $(ODIR)/mclin.o $(ODIR)/mcgeo.o $(ODIR)/lrotations.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/rotate3dbin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/rotate3dbin

$(BDIR)/rotatebin:	$(CDIR)/rotatebin.c $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/mcgeo.h $(IDIR)/lrotations.h $(OBJ_COMMON) $(ODIR)/mclin.o $(ODIR)/mcgeo.o $(ODIR)/lrotations.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/rotatebin.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/rotatebin

$(BDIR)/rotse:	$(CDIR)/rotse.c $(IDIR)/mcimage.h $(IDIR)/lsym.h $(OBJ_COMMON) $(ODIR)/lsym.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/rotse.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/rotse

$(BDIR)/selrect:	$(CDIR)/selrect.c $(IDIR)/mcimage.h $(IDIR)/lselrect.h $(OBJ_COMMON) $(ODIR)/lselrect.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/selrect.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/selrect

$(BDIR)/surrect:	$(CDIR)/surrect.c $(IDIR)/lgeo.h $(IDIR)/mcgeo.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/llabelextrema.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o $(ODIR)/mcgeo.o $(ODIR)/mctopo.o $(ODIR)/mclin.o $(ODIR)/lgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/surrect.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/surrect

$(BDIR)/sym:	$(CDIR)/sym.c $(IDIR)/mcimage.h $(IDIR)/lsym.h $(OBJ_COMMON) $(ODIR)/lsym.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/sym.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/sym

$(BDIR)/symse:	$(CDIR)/symse.c $(IDIR)/mcimage.h $(IDIR)/lsym.h $(OBJ_COMMON) $(ODIR)/lsym.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/symse.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/symse

$(BDIR)/traceellipses:	$(CDIR)/traceellipses.c $(IDIR)/mcimage.h $(IDIR)/lbresen.h $(OBJ_COMMON) $(ODIR)/lbresen.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/traceellipses.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/traceellipses

$(BDIR)/warp:	$(CDIR)/warp.c $(IDIR)/mcimage.h $(IDIR)/lwarp.h $(OBJ_COMMON) $(ODIR)/mcgeo.o $(ODIR)/lwarp.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/warp.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/warp

$(BDIR)/zoom:	$(CDIR)/zoom.c $(IDIR)/mcimage.h $(IDIR)/lzoom.h $(OBJ_COMMON) $(ODIR)/lzoom.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/zoom.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/zoom

$(BDIR)/zoomint:	$(CDIR)/zoomint.c $(IDIR)/mcimage.h $(IDIR)/lzoomint.h $(OBJ_COMMON) $(ODIR)/lzoomint.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/zoomint.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/zoomint

$(BDIR)/zoomrgb:	$(CDIR)/zoomrgb.c $(IDIR)/mcimage.h $(IDIR)/lzoom.h $(OBJ_COMMON) $(ODIR)/lzoom.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/zoomrgb.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/zoomrgb

# *********************************
# HISTO
# *********************************
$(BDIR)/comphisto:	$(CDIR)/comphisto.c $(IDIR)/lcomphisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/comphisto.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/comphisto

$(BDIR)/countvalues:	$(CDIR)/countvalues.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/countvalues.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/countvalues

$(BDIR)/egalise:	$(CDIR)/egalise.c $(IDIR)/legalise.h $(IDIR)/mcfah.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/egalise.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/egalise

$(BDIR)/histo:	$(CDIR)/histo.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histo.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/histo

$(BDIR)/histo2:	$(CDIR)/histo2.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histo2.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/histo2

$(BDIR)/histoazimuth:	$(CDIR)/histoazimuth.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histoazimuth.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/histoazimuth

$(BDIR)/histoazimuth2:	$(CDIR)/histoazimuth2.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histoazimuth2.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/histoazimuth2

$(BDIR)/histodist:	$(CDIR)/histodist.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histodist.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/histodist

$(BDIR)/histoelevation:	$(CDIR)/histoelevation.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histoelevation.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/histoelevation

$(BDIR)/histoelevation2:	$(CDIR)/histoelevation2.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histoelevation2.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/histoelevation2

$(BDIR)/histopgm:	$(CDIR)/histopgm.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histopgm.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/histopgm

$(BDIR)/histolisse:	$(CDIR)/histolisse.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histolisse.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/histolisse

$(BDIR)/histscal:	$(CDIR)/histscal.c $(IDIR)/lhistscal.h $(IDIR)/mcfah.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histscal.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/histscal

$(BDIR)/histosieve:	$(CDIR)/histosieve.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histosieve.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/histosieve

$(BDIR)/histstretch:	$(CDIR)/histstretch.c $(IDIR)/lhistscal.h $(IDIR)/mcfah.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/histstretch.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/histstretch

$(BDIR)/otsu:	$(CDIR)/otsu.c $(IDIR)/lhisto.h $(IDIR)/lseuil.h $(IDIR)/mcimage.h $(ODIR)/lseuil.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/otsu.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/otsu

$(BDIR)/seuilOtsu:    $(CDIR)/seuilOtsu.c $(IDIR)/mcimage.h $(IDIR)/lseuil.h $(OBJ_COMMON) $(ODIR)/lseuil.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/seuilOtsu.c $(OBJ_COMMON) $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/seuilOtsu

$(BDIR)/printhisto:	$(CDIR)/printhisto.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/printhisto.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/printhisto

$(BDIR)/printstats:	$(CDIR)/printstats.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/printstats.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/printstats

$(BDIR)/relabel:	$(CDIR)/relabel.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/relabel.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/relabel

$(BDIR)/seuilhisto:	$(CDIR)/seuilhisto.c $(IDIR)/lhisto.h $(IDIR)/mcimage.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/seuilhisto.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/seuilhisto

$(BDIR)/stretch:	$(CDIR)/stretch.c $(IDIR)/mcimage.h $(IDIR)/lstretch.h $(OBJ_COMMON) $(ODIR)/lstretch.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/stretch.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/stretch

# *********************************
# SIGNAL
# *********************************

$(BDIR)/average1:	$(CDIR)/average1.c $(IDIR)/lstat.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/lstat.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/average1.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/average1

$(BDIR)/ccv:	$(CDIR)/ccv.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lstat.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lstat.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ccv.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ccv

$(BDIR)/convol:	$(CDIR)/convol.c $(IDIR)/mcimage.h $(IDIR)/lconvol.h $(OBJ_COMMON) $(ODIR)/lfft.o $(ODIR)/lcrop.o $(ODIR)/lconvol.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/convol.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/convol

$(BDIR)/convol3:	$(CDIR)/convol3.c $(IDIR)/mcimage.h $(IDIR)/lconvol3.h $(OBJ_COMMON) $(ODIR)/lconvol3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/convol3.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/convol3

$(BDIR)/diZenzo:	$(CDIR)/diZenzo.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lderiche.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lderiche.o $(ODIR)/ldiZenzo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/diZenzo.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/diZenzo

$(BDIR)/deriche:	$(CDIR)/deriche.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lderiche.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lderiche.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/deriche.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/deriche

$(BDIR)/deriche3d:	$(CDIR)/deriche3d.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lderiche.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lderiche.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/deriche3d.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/deriche3d

$(BDIR)/directionalfilter:	$(CDIR)/directionalfilter.c $(IDIR)/mcimage.h $(IDIR)/lconvol.h $(ODIR)/lfft.o $(OBJ_COMMON) $(ODIR)/lcrop.o $(ODIR)/lconvol.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/directionalfilter.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/directionalfilter

$(BDIR)/ecarttype:	$(CDIR)/ecarttype.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lstat.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lstat.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/ecarttype.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/ecarttype

$(BDIR)/entropy:	$(CDIR)/entropy.c $(IDIR)/mcimage.h $(IDIR)/larith.h $(IDIR)/lhisto.h
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/entropy.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/entropy

$(BDIR)/fft:	$(CDIR)/fft.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lcrop.h $(IDIR)/lfft.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lcrop.o $(ODIR)/lfft.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/fft.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/fft

$(BDIR)/gradientcd:	$(CDIR)/gradientcd.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lderiche.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lderiche.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/gradientcd.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/gradientcd

$(BDIR)/gaussianfilter:	$(CDIR)/gaussianfilter.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lderiche.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lderiche.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/gaussianfilter.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/gaussianfilter

$(BDIR)/laplacian:	$(CDIR)/laplacian.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lderiche.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lderiche.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/laplacian.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/laplacian

$(BDIR)/longestplateau:	$(CDIR)/longestplateau.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/longestplateau.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/longestplateau

$(BDIR)/shencastan:	$(CDIR)/shencastan.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lderiche.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lderiche.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/shencastan.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/shencastan

$(BDIR)/meanfilter:	$(CDIR)/meanfilter.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lmeanfilter.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lmeanfilter.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/meanfilter.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/meanfilter

$(BDIR)/standarddeviation1:	$(CDIR)/standarddeviation1.c $(IDIR)/lstat.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/lstat.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/standarddeviation1.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/standarddeviation1

$(BDIR)/variance1:	$(CDIR)/variance1.c $(IDIR)/lstat.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/lstat.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/variance1.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/variance1

# ===============================================================
# MESH3D
# ===============================================================

$(BDIR)/mcube:	$(CDIR)/mcube.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mcgeo.h $(IDIR)/mcmesh.h $(IDIR)/mciomesh.h $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(IDIR)/mcrbtp.h $(ODIR)/mcrbtp.o $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcgeo.o $(ODIR)/mcprobas.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/mcube.c $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcgeo.o $(ODIR)/mcprobas.o $(LIBS) -o $(BDIR)/mcube

$(BDIR)/meshaddnoise:	$(CDIR)/meshaddnoise.c $(IDIR)/mcmesh.h $(IDIR)/mciomesh.h $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcprobas.o
	$(CPP) $(CCFLAGS) -I$(IDIR) -I. $(CDIR)/meshaddnoise.c $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(ODIR)/mcgeo.o $(ODIR)/mcprobas.o $(LIBS) -o $(BDIR)/meshaddnoise

$(BDIR)/meshconvert:	$(CDIR)/meshconvert.c $(IDIR)/mcmesh.h $(IDIR)/mciomesh.h $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(ODIR)/mcprobas.o $(OBJ_COMMON)
			$(CPP) $(CCFLAGS) -I$(IDIR) -I. $(CDIR)/meshconvert.c $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(ODIR)/mcgeo.o $(ODIR)/mcprobas.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/meshconvert

$(BDIR)/meshflatten:	$(CDIR)/meshflatten.c $(IDIR)/lgeo.h $(IDIR)/mcgeo.h $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/llabelextrema.h $(IDIR)/lbresen.h $(IDIR)/mcmesh.h $(IDIR)/mciomesh.h $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(ODIR)/mcprobas.o $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o $(ODIR)/mcgeo.o $(ODIR)/mctopo.o $(ODIR)/mclin.o $(ODIR)/lgeo.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/meshflatten.c $(ODIR)/lgeo.o $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/mccodimage.o $(ODIR)/mcgeo.o $(ODIR)/mctopo.o $(ODIR)/mclin.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(ODIR)/mcprobas.o $(LIBS) -o $(BDIR)/meshflatten

$(BDIR)/meshregul:	$(CDIR)/meshregul.c $(IDIR)/mcmesh.h $(IDIR)/mciomesh.h $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(ODIR)/mcprobas.o
	$(CPP) $(CCFLAGS) -I$(IDIR) -I. $(CDIR)/meshregul.c $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(ODIR)/mcgeo.o $(ODIR)/mcprobas.o $(LIBS) -o $(BDIR)/meshregul

$(BDIR)/mcm2ac:	$(CDIR)/mcm2ac.c $(IDIR)/mcmesh.h $(IDIR)/mciomesh.h $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcprobas.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/mcm2ac.c $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(ODIR)/mcgeo.o $(ODIR)/mcprobas.o $(LIBS) -o $(BDIR)/mcm2ac

$(BDIR)/mcm2vtk:	$(CDIR)/mcm2vtk.c $(IDIR)/mcmesh.h $(IDIR)/mciomesh.h $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcprobas.o
	$(CPP) $(CCFLAGS) -I$(IDIR) $(CDIR)/mcm2vtk.c $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(ODIR)/mcgeo.o $(ODIR)/mcprobas.o $(LIBS) -o $(BDIR)/mcm2vtk

$(BDIR)/mcmbuild:	$(CDIR)/mcmbuild.c $(IDIR)/mcmesh.h $(IDIR)/mciomesh.h $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcprobas.o
	$(CPP) $(CCFLAGS) -I$(IDIR) -I. $(CDIR)/mcmbuild.c $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(ODIR)/mcgeo.o $(ODIR)/mcprobas.o $(LIBS) -o $(BDIR)/mcmbuild

$(BDIR)/surfels:	$(CDIR)/surfels.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mcgeo.h $(IDIR)/mcmesh.h $(IDIR)/mciomesh.h $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(IDIR)/mcrbtp.h $(ODIR)/mcrbtp.o $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcgeo.o $(ODIR)/mcprobas.o
	$(CPP) $(CCFLAGS) -I$(IDIR) -I. $(CDIR)/surfels.c $(ODIR)/mcmesh.o $(ODIR)/mciomesh.o $(ODIR)/ssexport.o $(ODIR)/mcrbtp.o $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mcgeo.o $(ODIR)/mcprobas.o $(LIBS) -o $(BDIR)/surfels

# *********************************
# DIVERS
# *********************************

$(BDIR)/axones:	$(CDIR)/axones.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mcgeo.h $(IDIR)/llabelplateaux.h $(IDIR)/lwarp.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcgeo.o $(ODIR)/llabelplateaux.o $(ODIR)/lwarp.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/axones.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/axones

$(BDIR)/bruite:	$(CDIR)/bruite.c $(IDIR)/mcimage.h $(IDIR)/mcprobas.h $(OBJ_COMMON) $(ODIR)/mcprobas.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/bruite.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/bruite

$(BDIR)/colorize:	$(CDIR)/colorize.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/colorize.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/colorize

$(BDIR)/interpolate:	$(CDIR)/interpolate.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(OBJ_COMMON) $(ODIR)/mccodimage.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/interpolate.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/interpolate

$(BDIR)/genbicol:	$(CDIR)/genbicol.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/genbicol.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/genbicol

$(BDIR)/genbini:	$(CDIR)/genbini.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lbresen.h $(IDIR)/larith.h $(IDIR)/lpropgeo.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/larith.o $(ODIR)/lpropgeo.o $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(ODIR)/mcprobas.o $(ODIR)/lbresen.o $(ODIR)/mcliste.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/genbini.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/genbini

$(BDIR)/genlut:	$(CDIR)/genlut.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mccolors.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mccolors.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/genlut.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/genlut

$(BDIR)/gradill:	$(CDIR)/gradill.c $(IDIR)/lgradill.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/lgradill.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/gradill.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/gradill

$(BDIR)/grid:	$(CDIR)/grid.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/grid.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/grid

$(BDIR)/randimage:	$(CDIR)/randimage.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/randimage.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/randimage

$(BDIR)/kanungonoise:	$(CDIR)/kanungonoise.c $(IDIR)/mcimage.h $(IDIR)/mcprobas.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/kanungonoise.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/kanungonoise

$(BDIR)/randpoints:	$(CDIR)/randpoints.c $(IDIR)/mcimage.h $(OBJ_COMMON) $(IDIR)/mcprobas.h $(ODIR)/mcprobas.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/randpoints.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/randpoints

$(BDIR)/randrgb:	$(CDIR)/randrgb.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/randrgb.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/randrgb

$(BDIR)/shake:	$(CDIR)/shake.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/shake.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/shake

$(BDIR)/showheader:	$(CDIR)/showheader.c $(IDIR)/mcimage.h $(OBJ_COMMON)
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/showheader.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/showheader

$(BDIR)/showpoint:	$(CDIR)/showpoint.c $(IDIR)/lpoint.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/lpoint.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/showpoint.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/showpoint

$(BDIR)/skew:	$(CDIR)/skew.c $(IDIR)/mcimage.h $(IDIR)/mcutil.h $(IDIR)/lskew.h $(OBJ_COMMON) $(ODIR)/lskew.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/skew.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/skew

$(BDIR)/seamcarving:	$(CDIR)/seamcarving.c $(IDIR)/mcimage.h $(ODIR)/mcimage.o $(IDIR)/lseamcarving.h $(ODIR)/lseamcarving.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/seamcarving.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/seamcarving

# *********************************
# EXPERIMENTAL
# *********************************

$(BDIR)/contours:	$(CDIR)/contours.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/llabelextrema.h $(IDIR)/lcontours.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/lcontours.o 
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/contours.c $(OBJ_COMMON) $(ODIR)/mctopo.o $(ODIR)/mccodimage.o $(ODIR)/lcontours.o $(ODIR)/llabelextrema.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mclifo.o $(LIBS) -o $(BDIR)/contours

$(BDIR)/dynrecons:	$(CDIR)/dynrecons.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mclifo.h $(IDIR)/lhisto.h $(IDIR)/llabelextrema.h $(IDIR)/lhtkern.h $(ODIR)/libpink.a
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/dynrecons.c $(ODIR)/libpink.a $(LIBS) -o $(BDIR)/dynrecons

$(BDIR)/jones:	$(CDIR)/jones.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mctree.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/ljones.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mctree.o $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/ljones.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/jones.c $(OBJ_COMMON) $(ODIR)/mcindic.o $(ODIR)/mclifo.o $(OBJMCFAH) $(ODIR)/mctree.o $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/ljones.o $(LIBS) -o $(BDIR)/jones

$(BDIR)/levialdi:	$(CDIR)/levialdi.c $(IDIR)/mctopo.h $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/llevialdi.h $(ODIR)/mctopo.o $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/llevialdi.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/levialdi.c $(ODIR)/mctopo.o $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/llevialdi.o $(LIBS) -o $(BDIR)/levialdi

$(BDIR)/reconsplateaux:	$(CDIR)/reconsplateaux.c $(IDIR)/mccodimage.h $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/lreconsplateaux.h $(ODIR)/mccodimage.o $(OBJ_COMMON) $(ODIR)/mcindic.o $(ODIR)/mclifo.o $(ODIR)/lreconsplateaux.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/reconsplateaux.c $(ODIR)/lreconsplateaux.o $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/reconsplateaux

$(BDIR)/regul:	$(CDIR)/regul.c $(IDIR)/lregul.h $(IDIR)/lhtkern.h $(IDIR)/mcimage.h $(IDIR)/mcindic.h $(IDIR)/mclifo.h $(IDIR)/mccodimage.h $(ODIR)/lregul.o $(OBJ_COMMON) $(ODIR)/mcindic.o $(ODIR)/mclifo.o $(OBJMCFAH) $(ODIR)/mctopo.o $(ODIR)/mccodimage.o $(ODIR)/lhtkern.o $(ODIR)/mcfifo.o $(ODIR)/llabelextrema.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/regul.c $(ODIR)/lregul.o $(OBJ_COMMON) $(ODIR)/mcindic.o $(ODIR)/mclifo.o $(OBJMCFAH) $(ODIR)/mctopo.o $(ODIR)/mccodimage.o $(ODIR)/lthkern.o $(ODIR)/mcfifo.o $(ODIR)/llabelextrema.o $(LIBS) -o $(BDIR)/regul

$(BDIR)/remspnoise:	$(CDIR)/remspnoise.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lremspnoise.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lremspnoise.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/remspnoise.c $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/lremspnoise.o $(LIBS) -o $(BDIR)/remspnoise

$(BDIR)/segmentlignes:	$(CDIR)/segmentlignes.c $(IDIR)/mclifo.h $(IDIR)/mcimage.h $(IDIR)/llabelextrema.h $(IDIR)/mcindic.h $(IDIR)/mcfah.h $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/lsegmentlignes.h $(ODIR)/mclifo.o $(OBJ_COMMON) $(ODIR)/llabelextrema.o $(ODIR)/mcindic.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/lsegmentlignes.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/segmentlignes.c $(ODIR)/mclifo.o $(OBJ_COMMON) $(ODIR)/mcindic.o $(ODIR)/llabelextrema.o $(OBJMCFAH) $(ODIR)/mccodimage.o $(ODIR)/mctopo.o $(ODIR)/lsegmentlignes.o $(LIBS) -o $(BDIR)/segmentlignes

$(BDIR)/split:	$(CDIR)/split.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldistgeo.h $(IDIR)/llabelextrema.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(ODIR)/ldistgeo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/split.c $(ODIR)/ldistgeo.o $(ODIR)/mccodimage.o $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/llabelextrema.o $(LIBS) -o $(BDIR)/split

$(BDIR)/souzabanon:	$(CDIR)/souzabanon.c $(IDIR)/larith.h $(IDIR)/mcimage.h $(OBJ_COMMON) $(ODIR)/larith.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/souzabanon.c $(ODIR)/larith.o $(OBJ_COMMON) $(LIBS) -o $(BDIR)/souzabanon



# ===============================================================
# fonctions de la librairie bdd
# ===============================================================

$(ODIR)/bdd1.alphacube.o:	$(DDIR)/bdd1.alphacube.c
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(DDIR)/bdd1.alphacube.c -o $(ODIR)/bdd1.alphacube.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/bdd1.alphacube.o

$(ODIR)/bdd2.alpha.o:	$(DDIR)/bdd2.alpha.c
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(DDIR)/bdd2.alpha.c -o $(ODIR)/bdd2.alpha.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/bdd2.alpha.o

$(ODIR)/bdd2.beta.o:	$(DDIR)/bdd2.beta.c
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(DDIR)/bdd2.beta.c -o $(ODIR)/bdd2.beta.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/bdd2.beta.o

$(ODIR)/bdd3.o:	$(DDIR)/bdd3.c
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(DDIR)/bdd3.c -o $(ODIR)/bdd3.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/bdd3.o

# les "operateurs":
# ===============================================================
# *********************************
# ARITH
# *********************************

$(ODIR)/larith.o:	$(LDIR)/larith.c $(IDIR)/larith.h $(IDIR)/mccodimage.h $(IDIR)/mcutil.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/larith.c -o $(ODIR)/larith.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/larith.o

# *********************************
# CONVERT
# *********************************

# *********************************
# MORPHO
# *********************************

$(ODIR)/lasft.o:	$(LDIR)/lasft.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldilateros.h $(IDIR)/lhtkern.h $(IDIR)/lmedialaxis.h $(IDIR)/lasft.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lasft.c -o $(ODIR)/lasft.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lasft.o

$(ODIR)/ldilateros.o:	$(LDIR)/ldilateros.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h $(IDIR)/ldilateros.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ldilateros.c -o $(ODIR)/ldilateros.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ldilateros.o

$(ODIR)/ldilateros3d.o:	$(LDIR)/ldilateros3d.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h $(IDIR)/ldilateros3d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ldilateros3d.c -o $(ODIR)/ldilateros3d.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ldilateros3d.o

$(ODIR)/ldir.o:	$(LDIR)/ldir.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h $(IDIR)/mcindic.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ldir.c -o $(ODIR)/ldir.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ldir.o

$(ODIR)/ldist.o:	$(LDIR)/ldist.c $(IDIR)/mccodimage.h $(IDIR)/mclifo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ldist.c -o $(ODIR)/ldist.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ldist.o

$(ODIR)/ldistgeo.o:	$(LDIR)/ldistgeo.c $(IDIR)/mccodimage.h $(IDIR)/mclifo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ldistgeo.c -o $(ODIR)/ldistgeo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ldistgeo.o

$(ODIR)/lfiltreordre.o:	$(LDIR)/lfiltreordre.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lfiltreordre.c -o $(ODIR)/lfiltreordre.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lfiltreordre.o

$(ODIR)/lgettree.o:	$(LDIR)/lgettree.c $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lgettree.c -o $(ODIR)/lgettree.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lgettree.o

$(ODIR)/llabelextrema.o:	$(LDIR)/llabelextrema.c $(IDIR)/mclifo.h $(IDIR)/mccodimage.h $(IDIR)/mcutil.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/llabelextrema.c -o $(ODIR)/llabelextrema.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llabelextrema.o

$(ODIR)/llocalextrema.o:	$(LDIR)/llocalextrema.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/llocalextrema.c -o $(ODIR)/llocalextrema.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llocalextrema.o

$(ODIR)/llabelplateaux.o:	$(LDIR)/llabelplateaux.c $(IDIR)/mclifo.h $(IDIR)/mccodimage.h $(IDIR)/mcutil.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/llabelplateaux.c -o $(ODIR)/llabelplateaux.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llabelplateaux.o

$(ODIR)/llabeltree.o:	$(LDIR)/llabeltree.c $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/llabeltree.c -o $(ODIR)/llabeltree.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llabeltree.o

$(ODIR)/lmedialaxis.o:	$(LDIR)/lmedialaxis.c $(IDIR)/mccodimage.h $(IDIR)/mcimage.h $(IDIR)/ltopotypes.h $(IDIR)/ldist.h $(IDIR)/avsimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lmedialaxis.c -o $(ODIR)/lmedialaxis.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lmedialaxis.o

$(ODIR)/lminima.o:	$(LDIR)/lminima.c $(IDIR)/mclifo.h $(IDIR)/mccodimage.h $(IDIR)/mcutil.h $(IDIR)/llabelextrema.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lminima.c -o $(ODIR)/lminima.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lminima.o

$(ODIR)/lvoronoilabelling.o:	$(LDIR)/lvoronoilabelling.c $(IDIR)/mccodimage.h $(IDIR)/mclifo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lvoronoilabelling.c -o $(ODIR)/lvoronoilabelling.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lvoronoilabelling.o

$(ODIR)/lfmm.o: $(LDIR)/lfmm.c $(IDIR)/lfmm.h $(IDIR)/pde_toolbox.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lfmm.c -o $(ODIR)/lfmm.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lfmm.o

$(ODIR)/bimage.o: $(LDIR)/bimage.c $(IDIR)/lfmm.h $(IDIR)/pde_toolbox.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/bimage.c -o $(ODIR)/bimage.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/bimage.o

$(ODIR)/bimage_utils.o: $(LDIR)/bimage_utils.c $(IDIR)/lfmm.h $(IDIR)/pde_toolbox.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/bimage_utils.c -o $(ODIR)/bimage_utils.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/bimage_utils.o

$(ODIR)/lstb_io.o: $(LDIR)/lstb_io.c $(IDIR)/lfmm.h $(IDIR)/pde_toolbox.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lstb_io.c -o $(ODIR)/lstb_io.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lstb_io.o

$(ODIR)/fmmgeodist.o: $(LDIR)/fmmgeodist.c $(IDIR)/lfmm.h $(IDIR)/pde_toolbox.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/fmmgeodist.c -o $(ODIR)/fmmgeodist.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/fmmgeodist.o

$(ODIR)/lfmmdist.o: $(LDIR)/lfmmdist.c $(IDIR)/lfmm.h $(IDIR)/pde_toolbox.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lfmmdist.c -o $(ODIR)/lfmmdist.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lfmmdist.o

# *********************************
# CONNECT
# *********************************

$(ODIR)/lattribarea.o:	$(LDIR)/lattribarea.c $(LDIR)/lattrib.h $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mcfahsalembier.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lattribarea.c -o $(ODIR)/lattribarea.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lattribarea.o

$(ODIR)/lattribheight.o:	$(LDIR)/lattribheight.c $(LDIR)/lattrib.h $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mcfahsalembier.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lattribheight.c -o $(ODIR)/lattribheight.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lattribheight.o

$(ODIR)/lattribvol.o:	$(LDIR)/lattribvol.c $(LDIR)/lattrib.h $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mcfahsalembier.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lattribvol.c -o $(ODIR)/lattribvol.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lattribvol.o

$(ODIR)/ldynamique.o:	$(LDIR)/ldynamique.c $(LDIR)/lattrib.h $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mcfahsalembier.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ldynamique.c -o $(ODIR)/ldynamique.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ldynamique.o

$(ODIR)/ldynamique_grimaud.o:	$(LDIR)/ldynamique_grimaud.c $(LDIR)/lattrib.h $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mcfahsalembier.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ldynamique_grimaud.c -o $(ODIR)/ldynamique_grimaud.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ldynamique_grimaud.o

$(ODIR)/lgeodesic.o:	$(LDIR)/lgeodesic.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mcfifo.h $(IDIR)/mcindic.h $(IDIR)/lgeodesic.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lgeodesic.c -o $(ODIR)/lgeodesic.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lgeodesic.o

$(ODIR)/llpemeyer.o:	$(LDIR)/llpemeyer.c $(IDIR)/mccodimage.h $(IDIR)/mcfah.h $(IDIR)/mcindic.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/llpemeyer.c -o $(ODIR)/llpemeyer.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llpemeyer.o

$(ODIR)/lpropgeo.o:	$(LDIR)/lpropgeo.c $(IDIR)/mccodimage.h $(IDIR)/mcfifo.h $(IDIR)/mcindic.h $(IDIR)/mcprobas.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lpropgeo.c -o $(ODIR)/lpropgeo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lpropgeo.o

$(ODIR)/lsaliency.o:	$(LDIR)/lsaliency.c $(IDIR)/lsaliency.h 
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lsaliency.c -o $(ODIR)/lsaliency.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lsaliency.o

$(ODIR)/lsegreconsheight.o:	$(LDIR)/lsegreconsheight.c $(LDIR)/lattrib.h $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mcfahsalembier.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lsegreconsheight.c -o $(ODIR)/lsegreconsheight.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lsegreconsheight.o

$(ODIR)/lsegment3d.o:	$(LDIR)/lsegment3d.c $(IDIR)/mccodimage.h $(IDIR)/mcfah.h $(IDIR)/mccbt.h $(IDIR)/mcindic.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lsegment3d.c -o $(ODIR)/lsegment3d.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lsegment3d.o

$(ODIR)/lsegmentnum.o:	$(LDIR)/lsegmentnum.c $(IDIR)/mccodimage.h $(IDIR)/mcfah.h $(IDIR)/mccbt.h $(IDIR)/mcindic.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lsegmentnum.c -o $(ODIR)/lsegmentnum.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lsegmentnum.o

$(ODIR)/lwshedtopo.o:	$(LDIR)/lwshedtopo.c $(LDIR)/lattrib.h $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mcfahsalembier.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lwshedtopo.c -o $(ODIR)/lwshedtopo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lwshedtopo.o

# *********************************
# TOPO
# *********************************

$(ODIR)/lborder.o:	$(LDIR)/lborder.c $(IDIR)/mccodimage.h $(IDIR)/mctopo.h 
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lborder.c -o $(ODIR)/lborder.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lborder.o

$(ODIR)/lcurves.o:	$(LDIR)/lcurves.c $(IDIR)/lcurves.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lcurves.c -o $(ODIR)/lcurves.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lcurves.o

$(ODIR)/lfiltrestopo.o:	$(LDIR)/lfiltrestopo.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lfiltrestopo.c -o $(ODIR)/lfiltrestopo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lfiltrestopo.o

$(ODIR)/lfermetrous3d.o:	$(LDIR)/lfermetrous3d.c $(IDIR)/mccodimage.h $(IDIR)/mcfahpure.h $(IDIR)/mctopo3d.h $(IDIR)/mctopo3d_table.h $(IDIR)/ldist.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lfermetrous3d.c -o $(ODIR)/lfermetrous3d.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lfermetrous3d.o

$(ODIR)/lhthiniso.o:	$(LDIR)/lhthiniso.c $(IDIR)/mccodimage.h $(IDIR)/mcimage.h $(IDIR)/mctopo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lhthiniso.c -o $(ODIR)/lhthiniso.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lhthiniso.o

$(ODIR)/lhtkern.o:	$(LDIR)/lhtkern.c $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mcfah.h $(IDIR)/mctopo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lhtkern.c -o $(ODIR)/lhtkern.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lhtkern.o

$(ODIR)/lhtkern3d.o:	$(LDIR)/lhtkern3d.c $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mctopo3d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lhtkern3d.c -o $(ODIR)/lhtkern3d.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lhtkern3d.o

$(ODIR)/lkern.o:	$(LDIR)/lkern.c $(IDIR)/mccodimage.h $(IDIR)/mcfahpure.h $(IDIR)/mctopo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lkern.c -o $(ODIR)/lkern.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lkern.o

$(ODIR)/llambdakern.o:	$(LDIR)/llambdakern.c $(IDIR)/mccodimage.h $(IDIR)/mcfifo.h $(IDIR)/mcfahpure.h $(IDIR)/mctopo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/llambdakern.c -o $(ODIR)/llambdakern.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llambdakern.o

$(ODIR)/llambdalevkern.o:	$(LDIR)/llambdakern.c $(IDIR)/mccodimage.h $(IDIR)/mcfifo.h $(IDIR)/mcfahpure.h $(IDIR)/mctopo.h
	$(CC) -c $(CCFLAGS) -DNIVEL -I$(IDIR) $(LDIR)/llambdakern.c -o $(ODIR)/llambdalevkern.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llambdalevkern.o

$(ODIR)/llambdasquel.o:	$(LDIR)/llambdakern.c $(IDIR)/mccodimage.h $(IDIR)/mcfifo.h $(IDIR)/mcfahpure.h $(IDIR)/mctopo.h
	$(CC) -c $(CCFLAGS) -DSQUEL -I$(IDIR) $(LDIR)/llambdakern.c -o $(ODIR)/llambdasquel.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llambdasquel.o

$(ODIR)/llpetopo.o:	$(LDIR)/llpetopo.c $(IDIR)/mccodimage.h $(IDIR)/mcfah.h $(IDIR)/mccbt.h $(IDIR)/mcindic.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/llpetopo.c -o $(ODIR)/llpetopo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llpetopo.o

$(ODIR)/llpetoporeg.o:	$(LDIR)/llpetoporeg.c $(IDIR)/mccodimage.h $(IDIR)/mcfah.h $(IDIR)/mccbt.h $(IDIR)/mcindic.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/llpetoporeg.c -o $(ODIR)/llpetoporeg.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llpetoporeg.o

$(ODIR)/lnbtopo.o:	$(LDIR)/lnbtopo.c $(IDIR)/mccodimage.h $(IDIR)/mctopo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lnbtopo.c -o $(ODIR)/lnbtopo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lnbtopo.o

$(ODIR)/lnbvois.o:	$(LDIR)/lnbvois.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lnbvois.c -o $(ODIR)/lnbvois.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lnbvois.o

$(ODIR)/lskel2graph.o:	$(LDIR)/lskel2graph.c $(IDIR)/lskel2graph.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lskel2graph.c -o $(ODIR)/lskel2graph.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lskel2graph.o

$(ODIR)/lskelcurv.o:	$(LDIR)/lskelcurv.c $(IDIR)/lskelcurv.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lskelcurv.c -o $(ODIR)/lskelcurv.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lskelcurv.o

$(ODIR)/lseltopo.o:	$(LDIR)/lseltopo.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mckhalimsky3d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lseltopo.c -o $(ODIR)/lseltopo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lseltopo.o

$(ODIR)/lsquel.o:	$(LDIR)/lsquel.c $(IDIR)/mccodimage.h $(IDIR)/mcfifo.h $(IDIR)/mctopo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lsquel.c -o $(ODIR)/lsquel.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lsquel.o

$(ODIR)/lsquelbin.o:	$(LDIR)/lsquelbin.c $(IDIR)/mccodimage.h $(IDIR)/mcfifo.h $(IDIR)/mctopo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lsquelbin.c -o $(ODIR)/lsquelbin.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lsquelbin.o

$(ODIR)/lskeletons.o:	$(LDIR)/lskeletons.c $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mcrbt.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lskeletons.c -o $(ODIR)/lskeletons.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lskeletons.o

$(ODIR)/lskel_ACK3a.o:	$(LDIR)/lskel_ACK3a.c $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lskel_ACK3a.c -o $(ODIR)/lskel_ACK3a.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lskel_ACK3a.o

$(ODIR)/lskelpar.o:	$(LDIR)/lskelpar.c $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lskelpar.c -o $(ODIR)/lskelpar.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lskelpar.o

$(ODIR)/lskelpar3d.o:	$(LDIR)/lskelpar3d.c $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lskelpar3d.c -o $(ODIR)/lskelpar3d.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lskelpar3d.o

$(ODIR)/lskelpar3d_others.o:	$(LDIR)/lskelpar3d_others.c $(IDIR)/mccodimage.h $(IDIR)/lskelpar3d_others.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lskelpar3d_others.c -o $(ODIR)/lskelpar3d_others.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lskelpar3d_others.o

$(ODIR)/lsquelval.o:	$(LDIR)/lsquelval.c $(IDIR)/mccodimage.h $(IDIR)/mctopo.h $(IDIR)/mctopo3d.h $(IDIR)/mcrbt.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lsquelval.c -o $(ODIR)/lsquelval.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lsquelval.o

$(ODIR)/ltopotypes.o:	$(LDIR)/ltopotypes.c $(IDIR)/mccodimage.h $(IDIR)/mctopo.h 
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ltopotypes.c -o $(ODIR)/ltopotypes.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ltopotypes.o

$(ODIR)/DirectionalSkeletonizer.o:	$(LDIR)/DirectionalSkeletonizer.cxx $(IDIR)/DirectionalSkeletonizer.hpp $(IDIR)/FIFO.hpp 
#	$(CPP) -std=c++0x -c $(CCFLAGS) -I$(IDIR) $(LDIR)/DirectionalSkeletonizer.cxx -o $(ODIR)/DirectionalSkeletonizer.o
	$(CPP) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/DirectionalSkeletonizer.cxx -o $(ODIR)/DirectionalSkeletonizer.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/DirectionalSkeletonizer.o

# *********************************
# ORDRES
# *********************************

$(ODIR)/l2dkhalimsky.o:	$(LDIR)/l2dkhalimsky.c $(IDIR)/mccodimage.h $(IDIR)/mckhalimsky2d.h $(IDIR)/l2dkhalimsky.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/l2dkhalimsky.c -o $(ODIR)/l2dkhalimsky.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/l2dkhalimsky.o

$(ODIR)/l2dcollapse.o:	$(LDIR)/l2dcollapse.c $(IDIR)/mccodimage.h $(IDIR)/mckhalimsky2d.h $(IDIR)/mcrlifo.h $(IDIR)/l2dcollapse.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/l2dcollapse.c -o $(ODIR)/l2dcollapse.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/l2dcollapse.o

$(ODIR)/l3dkhalimsky.o:	$(LDIR)/l3dkhalimsky.c $(IDIR)/mccodimage.h $(IDIR)/mckhalimsky3d.h $(IDIR)/l3dkhalimsky.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/l3dkhalimsky.c -o $(ODIR)/l3dkhalimsky.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/l3dkhalimsky.o

$(ODIR)/l3dcollapse.o:	$(LDIR)/l3dcollapse.c $(IDIR)/mccodimage.h $(IDIR)/mckhalimsky3d.h $(IDIR)/l3dcollapse.h $(IDIR)/mcrlifo.h $(IDIR)/mcgraphe.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/l3dcollapse.c -o $(ODIR)/l3dcollapse.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/l3dcollapse.o


# *********************************
# DRAW
# *********************************

$(ODIR)/ldraw.o:	$(LDIR)/ldraw.c $(IDIR)/ldraw.h $(IDIR)/mccodimage.h $(IDIR)/mcutil.h $(IDIR)/lbresen.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ldraw.c -o $(ODIR)/ldraw.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ldraw.o

# *********************************
# GEO
# *********************************

$(ODIR)/lbdigitalline.o:	$(LDIR)/lbdigitalline.cxx $(IDIR)/lbdigitalline.h
	$(CPP) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lbdigitalline.cxx -o $(ODIR)/lbdigitalline.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lbdigitalline.o

$(ODIR)/ltangents.o:	$(LDIR)/ltangents.cxx $(IDIR)/ltangents.h $(IDIR)/lbdigitalline.h
	$(CPP) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ltangents.cxx -o $(ODIR)/ltangents.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ltangents.o

$(ODIR)/lattribute.o:	$(LDIR)/lattribute.c $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mctopo.h $(IDIR)/lattribute.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lattribute.c -o $(ODIR)/lattribute.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lattribute.o

$(ODIR)/lballincl.o:	$(LDIR)/lballincl.c $(IDIR)/lballincl.h 
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lballincl.c -o $(ODIR)/lballincl.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lballincl.o

$(ODIR)/lbarycentre.o:	$(LDIR)/lbarycentre.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lbarycentre.c -o $(ODIR)/lbarycentre.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lbarycentre.o

$(ODIR)/lbresen.o:	$(LDIR)/lbresen.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h $(IDIR)/mcliste.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lbresen.c -o $(ODIR)/lbresen.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lbresen.o

$(ODIR)/lconvexhull.o:	$(LDIR)/lconvexhull.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lconvexhull.c -o $(ODIR)/lconvexhull.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lconvexhull.o

$(ODIR)/lcrop.o:	$(LDIR)/lcrop.c $(IDIR)/mccodimage.h $(IDIR)/lcrop.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lcrop.c -o $(ODIR)/lcrop.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lcrop.o

$(ODIR)/libcrop.o: $(LDIR)/libcrop.c $(IDIR)/libcrop.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) -D_LARGEFILE64_SOURCE=1 -D_FILE_OFFSET_BITS=64 $< -o $(ODIR)/libcrop.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/libcrop.o

$(ODIR)/ldetectcercles.o:	$(LDIR)/ldetectcercles.c $(IDIR)/mccodimage.h $(IDIR)/lbresen.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ldetectcercles.c -o $(ODIR)/ldetectcercles.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ldetectcercles.o

$(ODIR)/ldirections.o:	$(LDIR)/ldirections.c $(IDIR)/mccodimage.h $(IDIR)/mclifo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ldirections.c -o $(ODIR)/ldirections.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ldirections.o

$(ODIR)/leden.o:	$(LDIR)/leden.cxx $(IDIR)/mctopo.h $(IDIR)/leden.h
	$(CPP) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/leden.cxx -o $(ODIR)/leden.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/leden.o

$(ODIR)/lellipsefit.o:	$(LDIR)/lellipsefit.c $(IDIR)/mccodimage.h $(IDIR)/mclin.h $(IDIR)/lellipsefit.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lellipsefit.c -o $(ODIR)/lellipsefit.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lellipsefit.o

$(ODIR)/lrecalagerigide.o:	$(LDIR)/lrecalagerigide.c $(IDIR)/mccodimage.h $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/mcgeo.h $(IDIR)/lrecalagerigide.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lrecalagerigide.c -o $(ODIR)/lrecalagerigide.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lrecalagerigide.o

$(ODIR)/lrecalagerigide_translateplane.o:	$(LDIR)/lrecalagerigide_translateplane.c $(IDIR)/mccodimage.h $(IDIR)/mcimage.h $(IDIR)/mclin.h $(IDIR)/mcgeo.h $(IDIR)/lrecalagerigide_translateplane.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lrecalagerigide_translateplane.c -o $(ODIR)/lrecalagerigide_translateplane.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lrecalagerigide_translateplane.o

$(ODIR)/lellipseincl.o:	$(LDIR)/lellipseincl.c $(IDIR)/mccodimage.h $(IDIR)/mclin.h $(IDIR)/lellipseincl.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lellipseincl.c -o $(ODIR)/lellipseincl.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lellipseincl.o

$(ODIR)/lgeo.o:	$(LDIR)/lgeo.c $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mclin.h $(IDIR)/llabelextrema.h $(IDIR)/lgeo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lgeo.c -o $(ODIR)/lgeo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lgeo.o

$(ODIR)/lhoughcercles.o:	$(LDIR)/lhoughcercles.c $(IDIR)/mccodimage.h $(IDIR)/lbresen.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lhoughcercles.c -o $(ODIR)/lhoughcercles.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lhoughcercles.o

$(ODIR)/lisometry.o:	$(LDIR)/lisometry.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lisometry.c -o $(ODIR)/lisometry.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lisometry.o

$(ODIR)/llenoir.o:	$(LDIR)/llenoir.c $(IDIR)/mccodimage.h $(IDIR)/llenoir.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/llenoir.c -o $(ODIR)/llenoir.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llenoir.o

$(ODIR)/lmaxdiameter.o:	$(LDIR)/lmaxdiameter.c $(IDIR)/mccodimage.h $(IDIR)/mctopo.h 
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lmaxdiameter.c -o $(ODIR)/lmaxdiameter.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lmaxdiameter.o

$(ODIR)/lmoments.o:	$(LDIR)/lmoments.c $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/llabelextrema.h $(IDIR)/lmoments.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lmoments.c -o $(ODIR)/lmoments.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lmoments.o

$(ODIR)/loffset.o:	$(LDIR)/loffset.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/loffset.c -o $(ODIR)/loffset.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/loffset.o

$(ODIR)/lpoint.o:	$(LDIR)/lpoint.c $(IDIR)/lpoint.h $(IDIR)/mccodimage.h $(IDIR)/mcutil.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lpoint.c -o $(ODIR)/lpoint.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lpoint.o

$(ODIR)/lrotations.o:	$(LDIR)/lrotations.c $(IDIR)/mccodimage.h $(IDIR)/mcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lrotations.c -o $(ODIR)/lrotations.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lrotations.o

$(ODIR)/lselrect.o:	$(LDIR)/lselrect.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lselrect.c -o $(ODIR)/lselrect.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lselrect.o

$(ODIR)/lsym.o:	$(LDIR)/lsym.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lsym.c -o $(ODIR)/lsym.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lsym.o

$(ODIR)/lwarp.o:	$(LDIR)/lwarp.c $(IDIR)/mccodimage.h $(IDIR)/mcgeo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lwarp.c -o $(ODIR)/lwarp.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lwarp.o

$(ODIR)/lvoronoi.o:	$(LDIR)/lvoronoi.c $(IDIR)/mccodimage.h $(IDIR)/mcgeo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lvoronoi.c -o $(ODIR)/lvoronoi.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lvoronoi.o

$(ODIR)/lzoom.o:	$(LDIR)/lzoom.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lzoom.c -o $(ODIR)/lzoom.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lzoom.o

$(ODIR)/lzoomint.o:	$(LDIR)/lzoomint.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lzoomint.c -o $(ODIR)/lzoomint.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lzoomint.o

# *********************************
# HISTO
# *********************************

$(ODIR)/lcomphisto.o:	$(LDIR)/lcomphisto.c $(IDIR)/lhisto.h $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lcomphisto.c -o $(ODIR)/lcomphisto.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lcomphisto.o

$(ODIR)/legalise.o:	$(LDIR)/legalise.c $(IDIR)/mcfah.h $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/legalise.c -o $(ODIR)/legalise.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/legalise.o

$(ODIR)/lhisto.o:	$(LDIR)/lhisto.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lhisto.c -o $(ODIR)/lhisto.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lhisto.o

$(ODIR)/lhistscal.o:	$(LDIR)/lhistscal.c $(IDIR)/mcfah.h $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lhistscal.c -o $(ODIR)/lhistscal.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lhistscal.o

$(ODIR)/lstretch.o:	$(LDIR)/lstretch.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lstretch.c -o $(ODIR)/lstretch.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lstretch.o

# *********************************
# SIGNAL
# *********************************

$(ODIR)/lstat.o:	$(LDIR)/lstat.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lstat.c -o $(ODIR)/lstat.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lstat.o

$(ODIR)/lconvol.o:	$(LDIR)/lconvol.c $(IDIR)/mccodimage.h $(IDIR)/lfft.h $(IDIR)/lcrop.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lconvol.c -o $(ODIR)/lconvol.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lconvol.o

$(ODIR)/lconvol3.o:	$(LDIR)/lconvol3.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lconvol3.c -o $(ODIR)/lconvol3.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lconvol3.o

$(ODIR)/ldiZenzo.o:	$(LDIR)/ldiZenzo.c $(IDIR)/mccodimage.h $(IDIR)/lderiche.h $(IDIR)/ldiZenzo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ldiZenzo.c -o $(ODIR)/ldiZenzo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ldiZenzo.o

$(ODIR)/lderiche.o:	$(LDIR)/lderiche.c $(IDIR)/mccodimage.h $(IDIR)/lderiche.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lderiche.c -o $(ODIR)/lderiche.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lderiche.o

$(ODIR)/lfft.o:	$(LDIR)/lfft.c $(IDIR)/mccodimage.h $(IDIR)/lfft.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lfft.c -o $(ODIR)/lfft.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lfft.o

$(ODIR)/lmeanfilter.o:	$(LDIR)/lmeanfilter.c $(IDIR)/mccodimage.h $(IDIR)/lmeanfilter.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lmeanfilter.c -o $(ODIR)/lmeanfilter.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lmeanfilter.o

# *********************************
# DIVERS
# *********************************

$(ODIR)/lgradill.o:	$(LDIR)/lgradill.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lgradill.c -o $(ODIR)/lgradill.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lgradill.o

$(ODIR)/lselndg.o:	$(LDIR)/lselndg.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lselndg.c -o $(ODIR)/lselndg.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lselndg.o

$(ODIR)/lseamcarving.o:	$(LDIR)/lseamcarving.c $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mctopo.h $(IDIR)/lseamcarving.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lseamcarving.c -o $(ODIR)/lseamcarving.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lseamcarving.o

$(ODIR)/lseuil.o:	$(LDIR)/lseuil.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lseuil.c -o $(ODIR)/lseuil.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lseuil.o

$(ODIR)/lskew.o:	$(LDIR)/lskew.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h $(IDIR)/lskew.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lskew.c -o $(ODIR)/lskew.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lskew.o

# *********************************
# EXPERIMENTAL
# *********************************
#
#$(ODIR)/lcontours.o:	$(LDIR)/lcontours.c $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mctopo.h
#	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lcontours.c -o $(ODIR)/lcontours.o
#	ar rcs $(ODIR)/libpink.a $(ODIR)/lcontours.o
#
#$(ODIR)/lepaisgeo.o:	$(LDIR)/lepaisgeo.c $(IDIR)/mccodimage.h $(IDIR)/mcfifo.h $(IDIR)/mctopo.h
#	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lepaisgeo.c -o $(ODIR)/lepaisgeo.o
#	ar rcs $(ODIR)/libpink.a $(ODIR)/lepaisgeo.o
#
#$(ODIR)/ljones.o:	$(LDIR)/ljones.c $(IDIR)/mccodimage.h $(IDIR)/mcfah.h $(IDIR)/mclifo.h $(IDIR)/mctree.h $(IDIR)/mcindic.h
#	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ljones.c -o $(ODIR)/ljones.o
#	ar rcs $(ODIR)/libpink.a $(ODIR)/ljones.o
#
#$(ODIR)/llevialdi.o:	$(LDIR)/llevialdi.c $(IDIR)/mctopo.h $(IDIR)/mccodimage.h
#	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/llevialdi.c -o $(ODIR)/llevialdi.o
#	ar rcs $(ODIR)/libpink.a $(ODIR)/llevialdi.o
#
#$(ODIR)/lreconsplateaux.o:	$(LDIR)/lreconsplateaux.c $(IDIR)/mccodimage.h $(IDIR)/mcutil.h $(IDIR)/mclifo.h
#	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lreconsplateaux.c -o $(ODIR)/lreconsplateaux.o
#	ar rcs $(ODIR)/libpink.a $(ODIR)/lreconsplateaux.o
#
#$(ODIR)/lregul.o:	$(LDIR)/lregul.c $(IDIR)/mclifo.h $(IDIR)/mccodimage.h $(IDIR)/mcutil.h
#	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lregul.c -o $(ODIR)/lregul.o
#	ar rcs $(ODIR)/libpink.a $(ODIR)/lregul.o
#
#$(ODIR)/lremspnoise.o:	$(LDIR)/lremspnoise.c $(IDIR)/mccodimage.h
#	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lremspnoise.c -o $(ODIR)/lremspnoise.o
#	ar rcs $(ODIR)/libpink.a $(ODIR)/lremspnoise.o
#
#$(ODIR)/lsegmentlignes.o:	$(LDIR)/lsegmentlignes.c $(IDIR)/mccodimage.h $(IDIR)/mcfah.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/llabelextrema.h
#	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lsegmentlignes.c -o $(ODIR)/lsegmentlignes.o
#	ar rcs $(ODIR)/libpink.a $(ODIR)/lsegmentlignes.o

# les "basiques":
# ===============================================================

$(ODIR)/mccbt.o:	$(LDIR)/mccbt.c $(IDIR)/mccbt.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mccbt.c -o $(ODIR)/mccbt.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mccbt.o

$(ODIR)/mcchrono.o:	$(LDIR)/mcchrono.c $(IDIR)/mcchrono.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcchrono.c -o $(ODIR)/mcchrono.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcchrono.o

$(ODIR)/mccodimage.o:	$(LDIR)/mccodimage.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mccodimage.c -o $(ODIR)/mccodimage.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mccodimage.o

$(ODIR)/mccolors.o:	$(LDIR)/mccolors.c $(IDIR)/mccolors.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mccolors.c -o $(ODIR)/mccolors.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mccolors.o

$(ODIR)/mccomptree.o:	$(LDIR)/mccomptree.c $(IDIR)/mccomptree.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mccomptree.c -o $(ODIR)/mccomptree.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mccomptree.o

$(ODIR)/mcdrawps.o:	$(LDIR)/mcdrawps.c $(IDIR)/mcdrawps.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcdrawps.c -o $(ODIR)/mcdrawps.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcdrawps.o

$(ODIR)/mcfah.o:	$(LDIR)/mcfah.c $(IDIR)/mccodimage.h $(IDIR)/mcfah.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcfah.c -o $(ODIR)/mcfah.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcfah.o

$(ODIR)/mcfahpure.o:	$(LDIR)/mcfahpure.c $(IDIR)/mccodimage.h $(IDIR)/mcfahpure.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcfahpure.c -o $(ODIR)/mcfahpure.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcfahpure.o

$(ODIR)/mcfahsalembier.o:	$(LDIR)/mcfahsalembier.c $(IDIR)/mccodimage.h $(IDIR)/mcfahsalembier.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcfahsalembier.c -o $(ODIR)/mcfahsalembier.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcfahsalembier.o

$(ODIR)/mcfifo.o:	$(LDIR)/mcfifo.c $(IDIR)/mcfifo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcfifo.c -o $(ODIR)/mcfifo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcfifo.o

$(ODIR)/mcfusion.o:	$(LDIR)/mcfusion.c $(IDIR)/mccodimage.h $(IDIR)/mcfusion.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcfusion.c -o $(ODIR)/mcfusion.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcfusion.o

$(ODIR)/mcgeo.o:	$(LDIR)/mcgeo.c $(IDIR)/mcgeo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcgeo.c -o $(ODIR)/mcgeo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcgeo.o

$(ODIR)/mcgraphe.o:	$(LDIR)/mcgraphe.c $(IDIR)/mcgraphe.h $(IDIR)/mcsort.h $(IDIR)/mcrbt.h $(IDIR)/mcfifo.h $(IDIR)/mclifo.h $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/ldraw.h 
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcgraphe.c -o $(ODIR)/mcgraphe.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcgraphe.o

$(ODIR)/mcimage.o:	$(LDIR)/mcimage.c $(IDIR)/mccodimage.h 
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcimage.c -o $(ODIR)/mcimage.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcimage.o

$(ODIR)/mcindic.o:	$(LDIR)/mcindic.c $(IDIR)/mccodimage.h $(IDIR)/mcindic.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcindic.c -o $(ODIR)/mcindic.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcindic.o

$(ODIR)/mciomesh.o:	$(LDIR)/mciomesh.c $(IDIR)/mciomesh.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mciomesh.c -o $(ODIR)/mciomesh.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mciomesh.o

$(ODIR)/mckhalimsky2d.o:	$(LDIR)/mckhalimsky2d.c $(IDIR)/mckhalimsky2d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mckhalimsky2d.c -o $(ODIR)/mckhalimsky2d.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mckhalimsky2d.o

$(ODIR)/mckhalimsky3d.o:	$(LDIR)/mckhalimsky3d.c $(IDIR)/mckhalimsky3d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mckhalimsky3d.c -o $(ODIR)/mckhalimsky3d.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mckhalimsky3d.o

$(ODIR)/mckhalimskyNd.o:	$(LDIR)/mckhalimskyNd.c $(IDIR)/mckhalimskyNd.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mckhalimskyNd.c -o $(ODIR)/mckhalimskyNd.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mckhalimskyNd.o

$(ODIR)/mclifo.o:	$(LDIR)/mclifo.c $(IDIR)/mclifo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mclifo.c -o $(ODIR)/mclifo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mclifo.o

$(ODIR)/mclin.o:	$(LDIR)/mclin.c $(IDIR)/mclin.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mclin.c -o $(ODIR)/mclin.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mclin.o

$(ODIR)/mcliste.o:	$(LDIR)/mcliste.c $(IDIR)/mcliste.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcliste.c -o $(ODIR)/mcliste.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcliste.o

$(ODIR)/mcmesh.o:	$(LDIR)/mcmesh.c $(IDIR)/mcmesh.h $(IDIR)/mcprobas.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcmesh.c -o $(ODIR)/mcmesh.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcmesh.o

$(ODIR)/mcpolygons.o:	$(LDIR)/mcpolygons.c $(IDIR)/mcpolygons.h $(IDIR)/mcprobas.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcpolygons.c -o $(ODIR)/mcpolygons.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcpolygons.o

$(ODIR)/mcpowell.o:	$(LDIR)/mcpowell.c $(IDIR)/mcpowell.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcpowell.c -o $(ODIR)/mcpowell.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcpowell.o

$(ODIR)/mcprobas.o:	$(LDIR)/mcprobas.c $(IDIR)/mcprobas.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcprobas.c -o $(ODIR)/mcprobas.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcprobas.o

$(ODIR)/mcrbt.o:	$(LDIR)/mcrbt.c $(IDIR)/mcrbt.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcrbt.c -o $(ODIR)/mcrbt.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcrbt.o

$(ODIR)/mcrbtp.o:	$(LDIR)/mcrbtp.c $(IDIR)/mcrbtp.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcrbtp.c -o $(ODIR)/mcrbtp.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcrbtp.o

$(ODIR)/mcrlifo.o:	$(LDIR)/mcrlifo.c $(IDIR)/mcrlifo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcrlifo.c -o $(ODIR)/mcrlifo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcrlifo.o

$(ODIR)/mcsegment.o:	$(LDIR)/mcsegment.c $(IDIR)/mcsegment.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcsegment.c -o $(ODIR)/mcsegment.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcsegment.o

$(ODIR)/mcset.o:	$(LDIR)/mcset.c $(IDIR)/mcset.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcset.c -o $(ODIR)/mcset.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcset.o

$(ODIR)/mcskel3d.o:	$(LDIR)/mcskel3d.c $(IDIR)/mcskel3d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcskel3d.c -o $(ODIR)/mcskel3d.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcskel3d.o

$(ODIR)/mcskel2d.o:	$(LDIR)/mcskel2d.c $(IDIR)/mcskel2d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcskel2d.c -o $(ODIR)/mcskel2d.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcskel2d.o

$(ODIR)/mcskel2d3d.o:	$(LDIR)/mcskel2d3d.c $(IDIR)/mcskel2d3d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcskel2d3d.c -o $(ODIR)/mcskel2d3d.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcskel2d3d.o

$(ODIR)/mcskelcurv.o:	$(LDIR)/mcskelcurv.c $(IDIR)/mcskelcurv.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcskelcurv.c -o $(ODIR)/mcskelcurv.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcskelcurv.o

$(ODIR)/mcsplines.o:	$(LDIR)/mcsplines.c $(IDIR)/mcsplines.h $(IDIR)/mclin.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcsplines.c -o $(ODIR)/mcsplines.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcsplines.o

$(ODIR)/mcsort.o:	$(LDIR)/mcsort.c $(IDIR)/mcsort.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcsort.c -o $(ODIR)/mcsort.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcsort.o

$(ODIR)/mctopo.o:	$(LDIR)/mctopo.c $(IDIR)/mctopo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mctopo.c -o $(ODIR)/mctopo.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mctopo.o

$(ODIR)/mctopo3d.o:	$(LDIR)/mctopo3d.c $(IDIR)/mctopo3d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mctopo3d.c -o $(ODIR)/mctopo3d.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mctopo3d.o

$(ODIR)/mctopo3d_table.o:	$(LDIR)/mctopo3d_table.c $(IDIR)/mctopo3d.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mctopo3d_table.c -o $(ODIR)/mctopo3d_table.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mctopo3d_table.o

$(ODIR)/mctree.o:	$(LDIR)/mctree.c $(IDIR)/mctree.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mctree.c -o $(ODIR)/mctree.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mctree.o

$(ODIR)/mcunionfind.o:	$(LDIR)/mcunionfind.c $(IDIR)/mcunionfind.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcunionfind.c -o $(ODIR)/mcunionfind.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/mcunionfind.o

# Andre Adds:
# ===============================================================
$(ODIR)/avscrop.o:	$(LDIR)/avscrop.c $(IDIR)/avscrop.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/avscrop.c -o $(ODIR)/avscrop.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/avscrop.o

$(ODIR)/avsimage.o:	$(LDIR)/avsimage.c $(IDIR)/mccodimage.h $(IDIR)/mcimage.h 
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/avsimage.c -o $(ODIR)/avsimage.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/avsimage.o

$(ODIR)/lhma.o:	$(LDIR)/lhma.c $(IDIR)/lhma.h $(IDIR)/llut.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lhma.c -o $(ODIR)/lhma.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lhma.o

$(ODIR)/llut.o:	$(LDIR)/llut.c $(IDIR)/avsimage.h $(IDIR)/llut.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/llut.c -o $(ODIR)/llut.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llut.o

$(BDIR)/hma:	$(CDIR)/hma.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/llut.h $(IDIR)/lhma.h $(ODIR)/avscrop.o $(ODIR)/lcrop.o $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/avsimage.o $(ODIR)/llut.o $(ODIR)/l2dkhalimsky.o $(ODIR)/l3dkhalimsky.o $(ODIR)/lhma.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/hma.c $(ODIR)/mccodimage.o $(OBJ_COMMON) $(ODIR)/avscrop.o $(ODIR)/avsimage.o $(ODIR)/lcrop.o $(ODIR)/llut.o $(ODIR)/lhma.o $(ODIR)/ldist.o $(ODIR)/ltopotypes.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(ODIR)/l2dkhalimsky.o $(ODIR)/l3dkhalimsky.o $(ODIR)/mckhalimsky2d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/bdd1.alphacube.o $(ODIR)/bdd2.alpha.o $(ODIR)/bdd2.beta.o $(ODIR)/bdd3.o $(ODIR)/mcindic.o $(LIBS) -o $(BDIR)/hma

# Jean Adds:
# ===============================================================

$(ODIR)/jclabelextrema.o:	$(LDIR)/jclabelextrema.c $(LDIR)/jccodimage.c $(IDIR)/jccodimage.h $(IDIR)/mclifo.h $(IDIR)/jclabelextrema.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/jclabelextrema.c -o $(ODIR)/jclabelextrema.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/jclabelextrema.o

$(ODIR)/llpemeyer4d.o:	$(LDIR)/llpemeyer4d.c $(IDIR)/mccodimage.h $(IDIR)/jccodimage.h $(IDIR)/mcfah.h $(IDIR)/mcindic.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/llpemeyer4d.c -o $(ODIR)/llpemeyer4d.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llpemeyer4d.o

$(ODIR)/llpeGA.o: 	$(LDIR)/llpeGA.c $(IDIR)/mccodimage.h $(IDIR)/mcimage.h $(IDIR)/jcimage.h $(IDIR)/jccodimage.h $(IDIR)/llpeGA.h 
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/llpeGA.c -o $(ODIR)/llpeGA.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/llpeGA.o

$(ODIR)/jcgraphes.o: 	$(LDIR)/jcgraphes.c $(IDIR)/jcgraphes.h 
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/jcgraphes.c -o $(ODIR)/jcgraphes.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/jcgraphes.o

$(ODIR)/lhierarchie.o: 	$(LDIR)/lhierarchie.c $(IDIR)/jcgraphes.h $(IDIR)/lhierarchie.h $(IDIR)/jccomptree.h $(IDIR)/llpeGA.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lhierarchie.c -o $(ODIR)/lhierarchie.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lhierarchie.o

$(ODIR)/lMSF.o:	$(LDIR)/lMSF.c $(IDIR)/mccodimage.h $(IDIR)/jccodimage.h $(LDIR)/mcrbt.c $(IDIR)/mcrbt.h $ $(IDIR)/lMSF.h 
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lMSF.c -o $(ODIR)/lMSF.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lMSF.o

$(ODIR)/jccodimage.o:		$(LDIR)/jccodimage.c $(IDIR)/mccodimage.h $(IDIR)/jccodimage.h $(IDIR)/jcimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/jccodimage.c -o $(ODIR)/jccodimage.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/jccodimage.o

$(ODIR)/jccomptree.o:	$(LDIR)/jccomptree.c $(IDIR)/jccomptree.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/jccomptree.c -o $(ODIR)/jccomptree.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/jccomptree.o

$(ODIR)/lppm2GA.o:	$(LDIR)/lppm2GA.c 
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lppm2GA.c -o $(ODIR)/lppm2GA.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lppm2GA.o

$(ODIR)/lga2khalimsky.o:	$(LDIR)/lga2khalimsky.c
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lga2khalimsky.c -o $(ODIR)/lga2khalimsky.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/lga2khalimsky.o

$(ODIR)/jcimage.o:	$(LDIR)/jcimage.c $(IDIR)/mccodimage.h $(IDIR)/jccodimage.h $(IDIR)/jcimage.h 
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/jcimage.c -o $(ODIR)/jcimage.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/jcimage.o

# Laszlo Adds:
# ===============================================================

$(ODIR)/ssexport.o:	$(LDIR)/ssexport.cxx $(IDIR)/mctopo.h $(IDIR)/ssexport.h
	$(CPP) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/ssexport.cxx -o $(ODIR)/ssexport.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/ssexport.o

$(ODIR)/liarwrap.o:	$(LIARDIR)/liarwrap.c
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LIARDIR)/liarwrap.c -o $(ODIR)/liarwrap.o
	ar rcs $(ODIR)/libpink.a $(ODIR)/liarwrap.o

# TABLE constructors
# ===============================================================

$(TDIR)/mhlut:	$(TDIR)/mhlut.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/lhma.h $(ODIR)/avscrop.o $(ODIR)/avsimage.o $(ODIR)/lcrop.o $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/llut.o $(ODIR)/lhma.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(TDIR)/mhlut.c $(ODIR)/mccodimage.o $(OBJ_COMMON) $(ODIR)/avscrop.o $(ODIR)/avsimage.o $(ODIR)/lcrop.o $(ODIR)/llut.o $(ODIR)/lhma.o $(ODIR)/ldist.o $(ODIR)/lseuil.o $(ODIR)/ltopotypes.o $(ODIR)/mctopo.o $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(LIBS) -o $(TDIR)/mhlut

$(TDIR)/SimpleTable:	$(TDIR)/SimpleTable.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mctopo3d.h $(OBJ_COMMON) $(ODIR)/mccodimage.o $(ODIR)/mctopo3d.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(TDIR)/SimpleTable.c $(ODIR)/mccodimage.o $(OBJ_COMMON) $(ODIR)/mctopo3d.o $(ODIR)/mclifo.o $(LIBS) -o $(TDIR)/SimpleTable
