/*
------------------------------ MNI Header ----------------------------------
@NAME       : extras.c
@DESCRIPTION: this file contains a number of extra non-deformation specific
               procedures.               
@CREATED    : Mon Nov  3, 1997 , Louis Collins
@MODIFIED   : not yet!
@VERSION    : $Id: extras.c,v 1.3 2000-03-15 08:42:46 stever Exp $
#-----------------------------------------------------------------------------
*/


#include <config.h>		
#include <internal_volume_io.h>	
#include <time.h>

public void report_time(long start_time, STRING text) 
{

   Real 
      time_total;
   long 
      present_time;
   STRING 
      time_total_string;

   ALLOC(time_total_string,512);

   present_time = time(NULL);

   time_total = (Real)(present_time - start_time);

#ifdef VOLIO_HAVE_2ARG_FORMAT_TIME
   time_total_string = format_time("%g %s", time_total);
#else
#  ifndef VOLIO_HAVE_3ARG_FORMAT_TIME
#    error Help!  Do not know how to invoke format_time.
#  endif
   /* This comes from a very very old VolumeIO library. */
   format_time( time_total_string,"%g %s", time_total);
#endif

   print ("\n%s : %s", text, time_total_string);

   if (time_total > 120)
      print (" (%d seconds)\n",(int)time_total);
   else
      print ("\n");

   FREE(time_total_string);
}

public void init_the_volume_to_zero(Volume volume)
{
    int             v0, v1, v2, v3, v4;
    Real            zero;
  

    zero = CONVERT_VALUE_TO_VOXEL(volume,0.0);
    
    BEGIN_ALL_VOXELS( volume, v0, v1, v2, v3, v4 )
      
      set_volume_voxel_value( volume, v0, v1, v2, v3, v4, zero );
    
    END_ALL_VOXELS

}

public Real get_volume_maximum_real_value(Volume volume)
{
    int             v0, v1, v2, v3, v4;
    Real            val,max;
  

    max = -DBL_MAX;

    BEGIN_ALL_VOXELS( volume, v0, v1, v2, v3, v4 )
      
      val = get_volume_real_value( volume, v0, v1, v2, v3, v4 );
      if (val > max) max = val;

    END_ALL_VOXELS

    return(max);
}


/*************************************************************************/
/* debug procedure called to save the deformation at each iterative step */

public void save_data(char *basename, int i, int j,
		      General_transform *transform)
{

  Status 
    status;
  STRING 
    comments,name;
  FILE *file;

  ALLOC(comments, 512);
  ALLOC(name, 512);
  (void)sprintf(comments,"step %d of %d of the non-linear estimation",i,j);
  
  (void)sprintf(name,"%s%d",basename,i);
  status = open_file_with_default_suffix(name,
					 get_default_transform_file_suffix(),
					 WRITE_FILE, ASCII_FORMAT, &file );
  
  if( status == OK )
    status = output_transform(file,
			      basename,
			      &i,
			      comments,
			      transform);
  
  if( status == OK )
    status = close_file( file );
  
  if (status!=OK)
    print ("Error saving %s%d\n",basename,i);

  FREE(name);
  FREE(comments);
}
