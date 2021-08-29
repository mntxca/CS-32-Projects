void removeOdds(vector<int>& v)
{
	if (v.empty())
		return;
	vector<int>::iterator iter1 = v.begin();
	while (iter1 != v.end())
	{
		if (*iter1 % 2 == 1)
			iter1 = v.erase(iter1);
		else
			iter1++;
	}
}