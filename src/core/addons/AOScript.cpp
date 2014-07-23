/*
 * Stellarium
 * Copyright (C) 2014 Marcos Cardinot
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335, USA.
 */

#include "AOScript.hpp"

AOScript::AOScript(StelAddOnDAO* pStelAddOnDAO)
	: m_pStelAddOnDAO(pStelAddOnDAO)
	, m_sScriptInstallDir(StelFileMgr::getUserDir() % "/scripts/")
{
	StelFileMgr::makeSureDirExistsAndIsWritable(m_sScriptInstallDir);
}

AOScript::~AOScript()
{
}

QStringList AOScript::checkInstalledAddOns() const
{
	QDir dir(m_sScriptInstallDir);
	dir.setFilter(QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);
	QStringList nameFilters;
	nameFilters << "*.ssc" << "*.sts";
	dir.setNameFilters(nameFilters);
	return dir.entryList();
}

bool AOScript::installFromFile(const QString& idInstall,
			       const QString& downloadFilepath) const
{
	QString suffix = "." % QFileInfo(downloadFilepath).suffix();
	if (suffix != ".ssc" && suffix != ".sts")
	{
		qWarning() << "Add-On Script: Unable to intall" << idInstall
			   << "The file found is not a .ssc or .sts";
		return false;
	}

	QString destination = m_sScriptInstallDir % "/" % idInstall % suffix;
	QFile(destination).remove();
	if (!QFile(downloadFilepath).copy(destination))
	{
		qWarning() << "Add-On Script: Unable to install" << idInstall;
		return false;
	}

	qDebug() << "Add-On Script: New script installed:" << idInstall;
	return true;
}

bool AOScript::uninstallAddOn(const QString &idInstall) const
{
	QFile ssc(m_sScriptInstallDir % idInstall % ".ssc");
	QFile sts(m_sScriptInstallDir % idInstall % ".sts");
	bool removed = true;

	if (ssc.exists())
	{
		removed = ssc.remove();
	}
	else if (sts.exists())
	{
		removed = sts.remove();
	}

	if (!removed)
	{
		qWarning() << "Add-On Scripts : Error! " << idInstall
			   << "could not be removed. ";
		return false;
	}

	qDebug() << "Add-On Scripts : Successfully removed" << idInstall;
	return true;
}
