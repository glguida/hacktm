#include <cmath>
#include <cassert>

#include "HackTM.h"
#include "Region.h"

namespace HackTM {

  Region::Region(const Space *inputspace, const Space *columnspace)
    : __inputSpace(inputspace), __columnSpace(columnspace), __inhibitionRadius(0)
  {
    __inputToColumn = new SpaceTransform(inputspace, columnspace);

    /* Create Columns */
    for ( unsigned i = 0; i < __columnSpace->getSize(); i++ )
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

  struct kthScore_closure
  {
    kthScore_closure(std::vector<Column*> &columns) : __columns(columns) {}

    void operator()(id_t id)
    {
      unsigned newscore = __columns[id]->getOverlap();

      /* Early exit if the k-sized vector is already full, and we're
	 lower than the current kth element. */
      if ( (scores.size() > __k) && (newscore <= scores[__k]) )
	return;

      scit = lower_bound(scores.begin(), scores.end(), newscore, std::greater<unsigned>());
      if ( scit - scores.begin() < __k )
	scit = scores.insert(scit, newscore);
    }

    void reset(unsigned k)
    {
      __k = k;
      scores.clear();
    }

    std::vector<unsigned> scores;
    std::vector<unsigned>::iterator scit;
    std::vector<Column*> &__columns;
    unsigned __k;
  };

  unsigned
  Region::kthScore(SubSpace &neighbors, unsigned k)
  {
    kthScore_closure func(columns); // XXX: Make this object specific.
    func.reset(k);
    neighbors.apply(func);

    if ( func.scores.size() >= k )
      return func.scores[k - 1];
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
      SubSpace neighbors(__columnSpace, c->getId(), __inhibitionRadius);
      
      minLocalActivity = kthScore(neighbors, htmconfig::desiredLocalActivity);

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
      id_t center = getColumnInputCenter(*it);
      scalar_t columnradius = scaleRadiusFromColumnSpace(1);
      scalar_t radius = 4 * columnradius;
      unsigned syns = columnradius * columnradius;

      p->populatePotentialSynapses(syns, center, radius);
      (*it)->proximalDendrite = p;
    }
  }

};

