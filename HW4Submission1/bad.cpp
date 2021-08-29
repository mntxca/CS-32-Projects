void removeBad(list<Movie*>& li)
{
	if (li.empty())
		return;
	list<Movie*>::iterator iter1 = li.begin();
	list<Movie*>::iterator savedForDelete;
	while (iter1 != li.end())
	{
		if ((*iter1)->rating() < 55)
		{
			savedForDelete = iter1;
			delete (*iter1);
			iter1 = li.erase(iter1);
		}
		else
			iter1++;
	}
}