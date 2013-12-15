using System;
using System.ComponentModel;
using System.Configuration;
using System.Data;
using System.IO;
using System.Web;

/// <summary>
/// Summary description for CustomerDataObject
/// </summary>
[DataObject(true)]
public class CustomersDataObject
{
    /// <summary>
    /// 
    /// </summary>
    private DataSet _customers;

    /// <summary>
    /// 
    /// </summary>
    public CustomersDataObject()
	{
        this._customers = HttpContext.Current.Session["Customers"] as DataSet;

        if (this._customers == null)
        {
            this._customers = new DataSet();
            this._customers.ReadXml(HttpContext.Current.Server.MapPath(@"App_Data\customers.xml"));

            HttpContext.Current.Session["Customers"] = this._customers;
        }
	}

    /// <summary>
    /// 
    /// </summary>
    private DataTable CustomerTable
    {
        get { return this._customers.Tables["customers"]; }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    [DataObjectMethod(DataObjectMethodType.Select)]
    public DataView Select()
    {
        this.CustomerTable.DefaultView.Sort = "CustomerID";
        return this.CustomerTable.DefaultView;
    }

    [DataObjectMethod(DataObjectMethodType.Delete)]
    public bool Delete()
    {
        return false;
    }
}
