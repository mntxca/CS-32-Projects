void listAll(const File* f, string path)  // two-parameter overload
{
    //You will write this code.    
    string updatedPath = path + "/" + f->name();
    cout << updatedPath << endl;
    if (f->files() == nullptr) //base case, which is that you are looking at a plain file
        ;
    else
    {
        vector<File*>::const_iterator iter = f->files()->begin();
        for (; iter != f->files()->end(); iter++)
            listAll(*iter, updatedPath);
    }
}