-rhreportmaxplayers sets the default max players to be reported before the instance has called rallyhere_stats_base()

-rhforcereportmaxplayers ignores any values set by rallyhere_stats_base() for max players and only reports this number given. Meant to be used by operations for misbehaving instances that are reporting the wrong max players value

-rhsimulatecurrentplayersat takes a tuple separated by a comma of seconds passed and the current player value to report. For example -rhsimulatecurrentplayersat=60,5 after will change the current players count to 5 after 60 seconds

-rhsimulatecurrentmaxplayersat= takes a tuple separated by a comma of seconds passed and the max player value to report. For example -rhsimulatemaxplayersat=60,5 after will change the max players count to 5 after 60 seconds

-rhsimulatelock is a comma separated list of stats value types to ignore the settings given by rallyhere_stats_base(). This is to allow the simulator complete control of these values.

-rhsimulateurl is a URL that can be called which returns the simulator parameters. This can be used to let an external service control the simualtion

-rhsicappendgrouptag= Will allow you to append tags that are reported to SIC. This allows us to append to any labels specified by the clients without getting in the way.

-rhsicregistrationurl
-rhsicloginurl
-rhsicpollurl

Set the URLs used for connecting to SIC. Normally these are derived from the rally here url, but in cases where that's not correct you can override them.
