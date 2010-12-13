#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

unsigned kthScore(std::vector<int>::iterator first, std::vector<int>::iterator last, unsigned k)
{
  std::vector<int>::iterator it;
  std::vector<unsigned> scores;
  std::vector<unsigned>::iterator scit;

  for ( it = first; it != last; it++ ) {
    unsigned newscore = *it;
    scit = upper_bound(scores.begin(), scores.end(), newscore, std::greater<unsigned>());
      if ( scit - scores.begin() < k ) {
	std::cout << "Inserting " << newscore << " at " << scit - scores.begin() << std::endl;
	scit = scores.insert(scit, newscore);
      }
    }
    if ( scores.size() >= k )
      return scores[k - 1];
    else
      return 0;
}

main()
{
  std::vector<int> a;

  for ( int i = 0; i < 200; i++ )
    a.push_back(i);

  std::random_shuffle(a.begin(), a.end());
  for ( int i = 1; i < 400; i++ )
    std::cout << kthScore(a.begin(), a.end(), i) << std::endl;


}
