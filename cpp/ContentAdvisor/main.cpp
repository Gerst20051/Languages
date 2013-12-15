#include <windows.h>
/*
Content Advisor Password Remover
Copyright (C) 2002 - 2007 Nir Sofer

Web site: http://www.nirsoft.net

The following utility removes the Content Advisor password in Internet Explorer (versions 4.x and above).
It simply does it by removing the encrypted password from the registry.
The encrypted password is located in HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\Ratings. 
The "Key" value contains the encrypted password in binary format.

In order to be able to enter into the Content Advisor after the password removal, 
you have to close IE and open it again. In some cases, you'll have to restart your computer.

*/

int WINAPI WinMain(
  HINSTANCE hInstance,  // handle to current instance
  HINSTANCE hPrevInstance,  
  LPSTR lpCmdLine,      // pointer to command line
  int nCmdShow          // show state of window
)
{
	// Ask the user if he want to remove the Content Advisor password.
	if (MessageBox(NULL, "Do you want to remove the Content Advisor password ?", 
					"Content Advisor Password Remover", 
					MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		HKEY hKey;
		
		// Opens the desired key in the registry. 
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Ratings", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			
			//Checks if the Content Advisor is protected by password  
			if (RegQueryValueEx(hKey, "Key", NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
			{
				//Try to delete the password.
				if (RegDeleteValue(hKey, "Key") == ERROR_SUCCESS)
					//Succeed !
					MessageBox(NULL, 
								"The Content Advisor password was successfully removed from you system.",
								"Content Advisor Password Remover", 
								MB_ICONINFORMATION | MB_OK);
				else
					MessageBox(NULL, 
								"Unable to remove the Content Advisor password.",
								"Content Advisor Password Remover", 
								MB_ICONEXCLAMATION | MB_OK);
			} else
				MessageBox(NULL, 
							"The Content Advisor is not protected by password", 
							"Content Advisor Password Remover", 
							MB_ICONEXCLAMATION | MB_OK);

		} else
			MessageBox(NULL, 
						"Unable to access the password in the registry.", 
						"Content Advisor Password Remover", 
						MB_ICONEXCLAMATION | MB_OK);


	}
	return 0;
}
 
