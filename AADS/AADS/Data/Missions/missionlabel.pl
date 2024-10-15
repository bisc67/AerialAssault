# MISSIONLABEL
while( <> )
{
    print $_;
    if( /(.*)(nameTag|name) = (".*")/ )
    {
        print $1,"label = ",$3,";\n";
    }
}
