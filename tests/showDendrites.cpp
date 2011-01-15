/* Copyright (c) 2010-2011 Gianluca Guida <glguida@gmail.com>
 *  
 * This software is released under the Numenta License for
 * Non-Commercial Use. You should have received a copy of the Numenta
 * License for Non-Commercial Use with this software (usually
 * contained in a file called LICENSE.TXT). If you don't, you can
 * download it from 
 * http://numenta.com/about-numenta/Numenta_Non-Commercial_License.txt.
 */

#include "HackTM.h"
#include "Introspection.h"
#include "ProximalDendrite.h"
#include "SpatialPooler.h"

using namespace hacktm;

/*
 * Print out potential synapses of a column at the center of the input
 * space in GNU plot data format.
 *
 * Usage: [s]plot "datafile" index <COLUMN> 
 *
 * See the GNUplot script 'showDendrites.gp'.
 */

int
main()
{
  Vector input(2, 1000);
  Vector col(2);
  col[0] = 20;
  col[1] = 20;
 
  Space inputSpace(input), columnSpace(col);
  SpatialPooler sp(&inputSpace, &columnSpace);

  IntrospectionLib::dumpAllPotentialSynapses(&sp);
 }

