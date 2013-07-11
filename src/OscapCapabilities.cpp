/*
 * Copyright 2013 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *      Martin Preisler <mpreisle@redhat.com>
 */

#include "OscapCapabilities.h"

#include <QRegExp>
#include <QStringList>

OscapCapabilities::OscapCapabilities()
{
    clear();
}

void OscapCapabilities::clear()
{
    mVersion = "Unknown";

    mBaselineSupport = false;
    mProgressReporting = false;
    mOnlineRemediation = false;
    mSourceDataStreams = false;
    mARFInput = false;
    mSCE = false;

    mXCCDFVersion = "Unknown";
    mOVALVersion = "Unknown";
    mCPEVersion = "Unknown";
    mSCEVersion = "Unknown";
}

static bool versionGreaterThan(const QString& a, const QString& b)
{
    const QStringList aSplit = a.split(".");
    const QStringList bSplit = a.split(".");

    QStringList::size_type pos = 0;

    while (pos < aSplit.size() && pos < bSplit.size())
    {
        if (aSplit[pos].toInt() < bSplit[pos].toInt())
            return false;

        ++pos;
    }

    return aSplit.size() > 0 && bSplit.size() > 0;
}

void OscapCapabilities::parse(const QString& mmv)
{
    clear();

    const QStringList lines = mmv.split("\n");

    if (lines.size() < 1)
        return; // TODO: Throw exception?

    const QStringList firstLine = lines[0].split(" ", QString::SkipEmptyParts);
    const QString& versionCandidate = firstLine.last();

    if (!versionCandidate.contains(QRegExp("^([0-9]+\\.){2,}[0-9]+$")))
        return; // TODO: Throw exception?

    mVersion = versionCandidate;

    // TODO: Pick a better version
    if (versionGreaterThan(mVersion, "0.8.0"))
        mBaselineSupport = true;

    if (versionGreaterThan(mVersion, "0.9.3"))
        mProgressReporting = true;

    if (versionGreaterThan(mVersion, "0.9.5"))
        mOnlineRemediation = true;

    if (versionGreaterThan(mVersion, "0.9.0"))
        mSourceDataStreams = true;

    if (lines.size() < 4 || !lines[3].contains("Supported specifications"))
        return; // TODO: Throw exception?

    QStringList::size_type linePos = 4;
    while (linePos < lines.size())
    {
        const QStringList versionLineSplit = lines[linePos].split(": ");

        if (versionLineSplit[0] == "XCCDF Version")
            mXCCDFVersion = versionLineSplit[1];
        else if (versionLineSplit[0] == "OVAL Version")
            mOVALVersion = versionLineSplit[1];
        else if (versionLineSplit[0] == "CPE Version")
            mCPEVersion = versionLineSplit[1];
        else if (versionLineSplit[0] == "Script check engine")
        {
            mSCE = true;
            mSCEVersion = versionLineSplit[1];
        }
        else
        {
            // TODO: Warn about unknown version
        }

        if (lines[linePos] == "")
            break; // End of the list

        ++linePos;
    }
}

const QString& OscapCapabilities::getOpenSCAPVersion() const
{
    return mVersion;
}

bool OscapCapabilities::baselineSupport() const
{
    return mBaselineSupport;
}

bool OscapCapabilities::progressReporting() const
{
    return mProgressReporting;
}

bool OscapCapabilities::onlineRemediation() const
{
    return mOnlineRemediation;
}

bool OscapCapabilities::sourceDatastreams() const
{
    return mSourceDataStreams;
}

bool OscapCapabilities::ARFInput() const
{
    return mARFInput;
}

const QString& OscapCapabilities::XCCDFVersion() const
{
    return mXCCDFVersion;
}

const QString& OscapCapabilities::OVALVersion() const
{
    return mOVALVersion;
}

const QString& OscapCapabilities::CPEVersion() const
{
    return mCPEVersion;
}