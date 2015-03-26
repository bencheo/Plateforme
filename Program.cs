    // gmcs <file>
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO;
using System.Text.RegularExpressions;

namespace Makefiles
{
  class JobFile
  {
    
    List<string> compilerList = new List<string>();
    List<string> optionsList = new List<string>();
						 public JobFile() { }
    public List<string> getCompilerList()
    {
      return compilerList;
    }
 
    public List<string> getOptionsList()
    {
      return optionsList;
    }

 

    public int SetCompilerList(string filename)
    {
      Regex rx = new Regex(@"compiler");
      //  MatchCollection matches;
      try
	{
	  using (StreamReader sr = new StreamReader(filename))
	  {
	    String line = sr.ReadToEnd();
	    rx = new Regex(@"compiler");
	    //	    matches = rx.Matches(line);
	    /*    if (matches.Count < 1)
		  {
		  Console.WriteLine(" No compiler \n");
		  return -1;
		  }*/
	    string replace = "compiler";
	    line.Replace(replace, " ");

	    string[] compilers = line.Split(new char[] { ' ' });
	    foreach (string s in compilers)
	    {
	      if (s.Trim() != "")
		compilerList.Add(s);
	    }
	  }
	}
      catch (Exception e)
	{
	  Console.WriteLine("The file could not be read :(");
	  Console.WriteLine(e.Message);
	}
      return 1;
    }
    
public int SetOptionList(string file_name)
    {
      Regex rx = new Regex(@"options");
      MatchCollection matches;

      try
	{
	  using (StreamReader sr = new StreamReader(file_name))
	  {
	    String line = sr.ReadLine();
	    rx = new Regex(@"optins");
	    matches = rx.Matches(line);
	    /*	    if (matches.Count < 1)
		    {
		    Console.WriteLine(" No options \n");
		    return -1;
		    }*/
	    line.Replace("options", "");

	    string[] options = line.Split(new char[] { ' ' });
	    foreach (string s in options)
	    {
	      if (s.Trim() != "")
		optionsList.Add(s);
	      else 
		break;
	    }
	  }
	}
      catch (Exception e)
	{
	  Console.WriteLine("The file  options could not be read:");
	  Console.WriteLine(e.Message);
	  return 0;
	}

      return 1;
    }
  }
    
    class Program
    {

	static void Main(string[] args)
      {  
	if(args.Length< 2)
	  {

	    Console.WriteLine("usage : <path> <filename>  \n \n please enter the path and the binary file name \n");
	   System.Environment.Exit(0);

	  }
	int platform = (int) Environment.OSVersion.Platform;
	bool islinux=false ;
        if( (platform == 4) || (platform == 6) || (platform == 128))
	  {
	    islinux=true;
	  }
	
	System.Diagnostics.Process make = new System.Diagnostics.Process();	
	make.StartInfo.FileName = "make";
	make.StartInfo.WorkingDirectory =@args[0];
	System.Diagnostics.Process clean = new System.Diagnostics.Process();
	clean.StartInfo.FileName = "make";
	clean.StartInfo.WorkingDirectory =@args[0];
	clean.StartInfo.Arguments="clean";
	System.Diagnostics.Process exe = new System.Diagnostics.Process();
       
	if(islinux)
	  exe.StartInfo.FileName = args[0]+args[1];
	else
	  exe.StartInfo.FileName="Bench.exe";
	exe.StartInfo.WorkingDirectory =@args[0];
	make.StartInfo.UseShellExecute = false;
	make.StartInfo.RedirectStandardOutput = true;

	JobFile jfcompile = new JobFile();
	JobFile jfoptions= new JobFile();
	jfcompile.SetCompilerList(args[0]+"compiler.txt");
	List<string> list = jfcompile.getCompilerList();
	jfoptions.SetOptionList(args[0]+"options.txt");
	List<string>listopt = jfoptions.getOptionsList();
	
	System.Diagnostics.Process cversion = new System.Diagnostics.Process();
	cversion.StartInfo.UseShellExecute = false;
	cversion.StartInfo.RedirectStandardOutput = true;

	string cxxflags="";
	string diag="";
	for (int i = 0; i < list.Count()-1; i++)
	  {
	    string cc="CXX="+list[i];
	    string compiler=list[i];
	    cversion.StartInfo.FileName = compiler;
	    cversion.StartInfo.Arguments=" --version";
	    cversion.Start();
		
	    string output = cversion.StandardOutput.ReadLine();
	    cversion.WaitForExit();  

	    Console.WriteLine( output);

	    for(int j =0; j<listopt.Count(); j++)
	      {
		  if(islinux)
			 cxxflags=" MyCXXFLAGS=' "+listopt[i]+"' ";
		  else
			 cxxflags=" MyCXXFLAGS="+listopt[i];
			
		string opt=cc+" "+ cxxflags;
		make.StartInfo.Arguments =opt;
		make.Start();
		make.WaitForExit();	       
	
		try
		  {
		    using (StreamReader sr = new StreamReader(args[0]+"/diagnostic.txt"))
		    {
		      diag= sr.ReadToEnd();
		      diag= diag.Replace("'", "");		    
		      diag= diag.Replace("\n"," ");
		      diag= diag.Replace("\"", "");		
		    }
		  }
		catch (Exception e)
		  {
		    Console.WriteLine("****************diagnostic.txt error*********");
		    Console.WriteLine(e.Message);
		  }
		//	Console.WriteLine(list[i]+ " ' "+output+ " ' "+listopt[j] + " ' "+diag +" ' ");
		if (islinux)
		  opt=list[i]+ "  ' "+output+ " ' "+listopt[j] + " ' "+diag +" ' ";
		else
		  opt = list[i] + " \"" +output +"\" " + listopt[j] +" \""+ diag +"\" ";
		
		exe.StartInfo.Arguments =opt;
		exe.Start();
		exe.WaitForExit(); 
		clean.Start();
		clean.WaitForExit();
	

	      }
	  }
      }
    }
}
