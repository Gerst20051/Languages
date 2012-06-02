/* 
    Copyright (c) 2010 Microsoft Corporation.  All rights reserved.
    Use of this sample source code is subject to the terms of the Microsoft license 
    agreement under which you licensed this sample source code and is provided AS-IS.
    If you did not accept the terms of the license agreement, you are not authorized 
    to use this sample source code.  For the terms of the license, please see the 
    license agreement between you and Microsoft.
    
*/

using System;
using System.IO.IsolatedStorage;
using System.Diagnostics;
using System.Collections.Generic;

namespace SettingsSample
{
    public class AppSettings
    {
        // Our isolated storage settings
        IsolatedStorageSettings isolatedStore;

        // The isolated storage key names of our settings
        const string CheckBoxSettingKeyName = "CheckBoxSetting";
        const string ListBoxSettingKeyName = "ListBoxSetting";
        const string RadioButton1SettingKeyName = "RadioButton1Setting";
        const string RadioButton2SettingKeyName = "RadioButton2Setting";
        const string RadioButton3SettingKeyName = "RadioButton3Setting";
        const string UsernameSettingKeyName = "UsernameSetting";
        const string PasswordSettingKeyName = "PasswordSetting";

        // The default value of our settings
        const bool CheckBoxSettingDefault = true;
        const int ListBoxSettingDefault = 0;
        const bool RadioButton1SettingDefault = true;
        const bool RadioButton2SettingDefault = false;
        const bool RadioButton3SettingDefault = false;
        const string UsernameSettingDefault = "";
        const string PasswordSettingDefault = "";

        /// <summary>
        /// Constructor that gets the application settings.
        /// </summary>
        public AppSettings()
        {
            try
            {
                // Get the settings for this application.
                isolatedStore = IsolatedStorageSettings.ApplicationSettings;

            }
            catch (Exception e)
            {
                Debug.WriteLine("Exception while using IsolatedStorageSettings: " + e.ToString());
            }
        }

        /// <summary>
        /// Update a setting value for our application. If the setting does not
        /// exist, then add the setting.
        /// </summary>
        /// <param name="Key"></param>
        /// <param name="value"></param>
        /// <returns></returns>
        public bool AddOrUpdateValue(string Key, Object value)
        {
            bool valueChanged = false;

            // If the key exists
            if (isolatedStore.Contains(Key))
            {
                // If the value has changed
                if (isolatedStore[Key] != value)
                {
                    // Store the new value
                    isolatedStore[Key] = value;
                    valueChanged = true;
                }
            }
            // Otherwise create the key.
            else
            {
                isolatedStore.Add(Key, value);
                valueChanged = true;
            }

           return valueChanged;
        }


        /// <summary>
        /// Get the current value of the setting, or if it is not found, set the 
        /// setting to the default setting.
        /// </summary>
        /// <typeparam name="valueType"></typeparam>
        /// <param name="Key"></param>
        /// <param name="defaultValue"></param>
        /// <returns></returns>
        public valueType GetValueOrDefault<valueType>(string Key, valueType defaultValue)
        {
            valueType value;

            // If the key exists, retrieve the value.
            if (isolatedStore.Contains(Key))
            {
                value = (valueType)isolatedStore[Key];
            }
            // Otherwise, use the default value.
            else
            {
                value = defaultValue;
            }

            return value;
        }


        /// <summary>
        /// Save the settings.
        /// </summary>
        public void Save()
        {
            isolatedStore.Save();
        }


        /// <summary>
        /// Property to get and set a CheckBox Setting Key.
        /// </summary>
        public bool CheckBoxSetting
        {
            get
            {
                return GetValueOrDefault<bool>(CheckBoxSettingKeyName, CheckBoxSettingDefault);
            }
            set
            {
                AddOrUpdateValue(CheckBoxSettingKeyName, value);
                Save();
            }
        }


        /// <summary>
        /// Property to get and set a ListBox Setting Key.
        /// </summary>
        public int ListBoxSetting
        {
            get
            {
                return GetValueOrDefault<int>(ListBoxSettingKeyName, ListBoxSettingDefault);
            }
            set
            {
                AddOrUpdateValue(ListBoxSettingKeyName, value);
                Save();
            }
        }


        /// <summary>
        /// Property to get and set a RadioButton Setting Key.
        /// </summary>
        public bool RadioButton1Setting
        {
            get
            {
                return GetValueOrDefault<bool>(RadioButton1SettingKeyName, RadioButton1SettingDefault);
            }
            set
            {
                AddOrUpdateValue(RadioButton1SettingKeyName, value);
                Save();
            }
        }


        /// <summary>
        /// Property to get and set a RadioButton Setting Key.
        /// </summary>
        public bool RadioButton2Setting
        {
            get
            {
                return GetValueOrDefault<bool>(RadioButton2SettingKeyName, RadioButton2SettingDefault);
            }
            set
            {
                AddOrUpdateValue(RadioButton2SettingKeyName, value);
                Save();
            }
        }

        /// <summary>
        /// Property to get and set a RadioButton Setting Key.
        /// </summary>
        public bool RadioButton3Setting
        {
            get
            {
                return GetValueOrDefault<bool>(RadioButton3SettingKeyName, RadioButton3SettingDefault);
            }
            set
            {
                AddOrUpdateValue(RadioButton3SettingKeyName, value);
                Save();
            }
        }

        /// <summary>
        /// Property to get and set a Username Setting Key.
        /// </summary>
        public string UsernameSetting
        {
            get
            {
                return GetValueOrDefault<string>(UsernameSettingKeyName, UsernameSettingDefault);
            }
            set
            {
                AddOrUpdateValue(UsernameSettingKeyName, value);
                Save();
            }
        }

        /// <summary>
        /// Property to get and set a Password Setting Key.
        /// </summary>
        public string PasswordSetting
        {
            get
            {
                return GetValueOrDefault<string>(PasswordSettingKeyName, PasswordSettingDefault);
            }
            set
            {
                AddOrUpdateValue(PasswordSettingKeyName, value);
                Save();
            }
        }

    }
}