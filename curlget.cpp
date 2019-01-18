/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2016, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* <DESC>
 * Very simple HTTP GET
 * </DESC>
 */
#include <stdlib.h>
#include <QString>
#include <QStringList>
#include <curl/curl.h>
#include "curlget.h"

static QString buffer;

//
//  libcurl write callback function
//

static int writer(char *data, size_t size, size_t nmemb, QString *writerData)
{
  if(writerData == NULL)
    return 0;

  writerData->append(data);

  return size * nmemb;
}

bool curlget (QString & data, QString & errmsg)
{
  CURL *curl;
  CURLcode res, code;

  curl = curl_easy_init();
  if(curl)
    {
      char errbuf[CURL_ERROR_SIZE];

      curl_easy_setopt(curl, CURLOPT_URL, 
		     "http://www.ipmango.com/api/myip"
		     );
      /* example.com is redirected, so we tell libcurl to follow redirection */
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

      code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
      if(code != CURLE_OK)
	{
	  // fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
	  errmsg = "Failed to set writer.";
	  return false;
	}

      buffer = "";
      code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
      if(code != CURLE_OK) {
	// fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
	errmsg = "Failed to set write data.";
	return false;
      }

      /* provide a buffer to store errors in */
      curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
 
      /* set the error buffer as empty before performing a request */
      errbuf[0] = 0;

      res = curl_easy_perform(curl); /* post away! */
 
      /* Check for errors */
      if(res != CURLE_OK)
	{
	  size_t len = strlen(errbuf);
	  QString resnum;
	  resnum.setNum(res);

	  errmsg = "Perform failed (code ";
	  errmsg += resnum;
	  errmsg += "): ";

	  if (len)
	    {
	      errmsg += errbuf;
	      errmsg += ((errbuf[len - 1] != '\n') ? "" : "");
	    }
	  else
	    {
	      errmsg += curl_easy_strerror(res);
	    }
	  /* always cleanup */
	  curl_easy_cleanup(curl);
	  return false;
	}
      else
	{
	  /* always cleanup */
	  curl_easy_cleanup(curl);
	}
    }

  QStringList parts;
  parts = buffer.split ("\n");
  data = parts[0];
  return true;
}

