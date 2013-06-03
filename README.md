test-inflate-rms
================

A simple cpp app that tries to decompress rpmsg files.

From Wikipedia: An rpmsg file is a restricted-permission message with an extension of rpmsg. It is used to implement IRM for Outlook messages with the aim of restricting certain actions such as the ability to forwarded or copy.
The rpmsg file is created by writing body, attachments and images to a compound file and then compressing and encrypting it to create a BLOB: the rpmsg file [5]. This resulting file is named message.rpmsg and is included as an attachment to a normal Outlook message.
--

This small app uncompress this BLOB and shows the content of XrML message.

When I was trying to understand the standard the covers RPSMG files, I saw some parts of the code posted here. However, It was not clear how to compile and put it to work.
So after sometime reading about MS-OXORMMS and understanding how the XrML message is compressed thru xLib, I was able to put all together and make the compilation work.

These are the urls used for this discovery:

http://blogs.msdn.com/b/openspecification/archive/2010/06/20/msg-file-format-rights-managed-email-message-part-2.aspx
http://social.msdn.microsoft.com/Forums/en-US/os_exchangeprotocols/thread/8e5b7d1d-253e-46dc-98f7-d6aee3c51bf0

