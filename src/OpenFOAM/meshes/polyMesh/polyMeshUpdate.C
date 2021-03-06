/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2010 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Description
    Update the polyMesh corresponding to the given map.

\*---------------------------------------------------------------------------*/

#include "polyMesh.H"
#include "mapPolyMesh.H"
#include "Time.H"
#include "globalMeshData.H"
#include "pointMesh.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::polyMesh::updateMesh(const mapPolyMesh& mpm)
{
    // Update boundaryMesh (note that patches themselves already ok)
    boundary_.updateMesh();

    // Update zones
    pointZones_.clearAddressing();
    faceZones_.clearAddressing();
    cellZones_.clearAddressing();

    // Update parallel data
    if (globalMeshDataPtr_)
    {
        globalMeshDataPtr_->updateMesh();
    }

    setInstance(time().timeName());

    // Map the old motion points if present
    if (oldPointsPtr_)
    {
        // Make a copy of the original points
        pointField oldMotionPoints = *oldPointsPtr_;

        pointField& newMotionPoints = *oldPointsPtr_;

        // Resize the list to new size
        newMotionPoints.setSize(points_.size());

        // Map the list
        newMotionPoints.map(oldMotionPoints, mpm.pointMap());

        // Any points created out-of-nothing get set to the current coordinate
        // for lack of anything better.
        forAll(mpm.pointMap(), newPointI)
        {
            if (mpm.pointMap()[newPointI] == -1)
            {
                newMotionPoints[newPointI] = points_[newPointI];
            }
        }
    }

    // Reset valid directions (could change by faces put into empty patches)
    geometricD_ = Vector<label>::zero;
    solutionD_ = Vector<label>::zero;


    // Hack until proper callbacks. Below are all the polyMesh-MeshObjects.

    // pointMesh
    if (thisDb().foundObject<pointMesh>(pointMesh::typeName))
    {
        const_cast<pointMesh&>
        (
            thisDb().lookupObject<pointMesh>
            (
                pointMesh::typeName
            )
        ).updateMesh(mpm);
    }
}


// ************************************************************************* //
