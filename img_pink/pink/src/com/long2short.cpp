/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/


/*! \file long2byte.c

\brief converts a "int32_t" image to a "ushort" image

<B>Usage:</B> long2byte in.pgm [mode] out.pgm

<B>Description:</B>

Depending on the value given for the (optional) parameter <B>mode</B>:
\li   <B>mode</B> = 0 (default) : for all x, out[x] = min(USHRT_MAX, in[x]).
\li   <B>mode</B> = 1 : for all x, out[x] = in[x] modulo USHRT_MAX.
\li   <B>mode</B> = 2 : scales values in the range 0-USHRT_MAX.
\li   <B>mode</B> = 4 : truncation of the square root in the range 0-255.
\li   <B>mode</B> = 5 : truncation of the log in the range 0-255.

<B>Types supported:</B> short_image (any D)

<B>Category:</B> convert
\ingroup  convert

\author Laszlo Marak, 2011
*/

























// LuM end of file
