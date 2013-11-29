if [ $LIBGL_DRIVERS_PATH ] 
then 
  if ! set | grep LIBGL_DRIVERS_PATH | grep /usr/lib/xorg/modules/dri/ > /dev/null 
  then 
    LIBGL_DRIVERS_PATH=$LIBGL_DRIVERS_PATH:/usr/lib/xorg/modules/dri/ 
    export LIBGL_DRIVERS_PATH 
  fi  
else 
  LIBGL_DRIVERS_PATH=/usr/lib/xorg/modules/dri/ 
  export LIBGL_DRIVERS_PATH 
fi 
