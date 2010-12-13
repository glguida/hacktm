#include <cmath>
#include <cassert>
#include "HackTMConfig.h"
#include "metric/Sphere.h"
#include "Region.h"

using namespace metric;

namespace HackTM {

  Region::Region(const MetricSpace *inputspace, const MetricSpace *columnspace)
    : __inputSpace(inputspace), __columnSpace(columnspace), __inhibitionRadius(0)
  {
    __inputToColumn = new MappingFunction(inputspace, columnspace);

    /* Create Columns */
    for ( unsigned i = 0; i < __columnSpace->size(); i++ )
      columns.push_back(new Column(i));

    __connectColumnsToInput();
  }

  Region::~Region()
  {
    delete __inputToColumn;

    for ( column_iterator it = columns.begin(); it != columns.end(); it++ )
      delete (*it);
  }

  void
  Region::updateColumnsOverlap(const BitVector &input)
  {
    column_iterator it;
    for ( it = columns.begin(); it != columns.end(); it++ ) {
      (*it)->updateOverlap(input);
    }
  }

  unsigned 
  Region::kthScore(Sphere::iterator first, Sphere::iterator last, unsigned k)
  {
    Sphere::iterator it;
    std::vector<unsigned> scores;
    std::vector<unsigned>::iterator scit;

    assert( k >= 1 );

    for ( it = first; it != last; it++ ) {
      unsigned newscore = columns[(*it)]->getOverlap();
      scit = upper_bound(scores.begin(), scores.end(), newscore, std::greater<unsigned>());
      if ( scit - scores.begin() < k )
	scit = scores.insert(scit, newscore);
    }
    if ( scores.size() >= k )
      return scores[k - 1];
    else
      return 0;
  }


  void
  Region::inhibitColumns()
  {
    column_iterator it;
    activeColumns.clear();

    for ( it = columns.begin(); it != columns.end(); it++ ) {
      Column *c = *it;
      unsigned minLocalActivity;
      Sphere s(__columnSpace, c->getId(), __inhibitionRadius);
      
      minLocalActivity = kthScore(s.begin(), s.end(), htmconfig::desiredLocalActivity);

      if ( c->getOverlap() > 0 && c->getOverlap() >= minLocalActivity )
	activeColumns.push_back(c);
    }
  }

  void
  Region::adjustProximalSynapses(const BitVector &input)
  {
    Region::activecol_iterator actit;
    for ( actit = activeColumns.begin(); actit != activeColumns.end(); actit++ ) {
      Column *c = *actit;
      c->proximalDendrite->adjustPotentialSynapses(input);
    }
  }

  void 
  Region::__connectColumnsToInput()
  {
    column_iterator it;
    for ( it = columns.begin(); it != columns.end(); it++ ) {
      ProximalDendrite *p = new ProximalDendrite(__inputSpace);
      unsigned center = getColumnInputCenter(*it),
	radius = scaleRadiusFromColumnSpace(4),
	syns = radius * radius / 10;

      p->populatePotentialSynapses(syns, center, radius);
      (*it)->proximalDendrite = p;
    }
  }

};

