#Process UK population data downloaded from worldpop.org/geodata
#Aim to project into a regular 100m grid in UK national grid projected co-ordinate system
#Comes from regular long-lat data at 0.00083 degree resolution
library(sf)
library(raster)
library(spDataLarge)
#above requires devtools::install_github("Nowosad/spDataLarge) - this also needs libxml2-devel
#see https://geocompr.robinlovelace.net/reproj-geo-data.html  
#This tif file has no projection info according to R - but the worldpop website says it is in WGS84
ukpop=raster("/home/moke/covid-outputs/gbr_ppp_2020.tif", package = "spDataLarge")
#what is the co-ordinate system? none! - 
crs(ukpop)
#query file like this if need data without loading...
#library(rgdal)
#GDALinfo("/home/moke/covid-outputs/gbr_ppp_2020.tif")
#WGS84 which is epsg:4326
projection(ukpop) = "+init=epsg:4326"
#UK national grid (now in metres) is 27700
ukgrid_pop=projectRaster(ukpop,crs="+init=epsg:27700",method="bilinear")
#resolution in x is about 50m, about 100m in y so aggregate in x to get 100m-ish cells
agrigrid=aggregate(ukgrid_pop,fact=c(2,1),fun=sum,na.rm=TRUE)
#change raster to 100m resolution in both x and y exactly 
reg_pop=projectExtent(agrigrid,crs="+init=epsg:27700")
res(reg_pop)=100
#resample doesn't seem to work well here? so re-project again
reg_pop=projectRaster(agrigrid,reg_pop,method="bilinear")
#reset missing data value
NAvalue(reg_pop) = -9999

reg_pop1km=aggregate(ukgrid_pop,fact=c(10,10),fun=sum,na.rm=TRUE)
#rescale so that the total is as the original
reg_pop=reg_pop *  cellStats(ukpop,stat="sum",na.rm=TRUE)/ cellStats(reg_pop,stat="sum",na.rm=TRUE)

#save out results as geotiff and ARC ascii grid
writeRaster(x = reg_pop,
            filename = "gbr_ppp_2020UKnatgrid100m.tif",
            datatype = "FLT4S",
            options = c("COMPRESS=DEFLATE"),
            overwrite = TRUE)
writeRaster(x = reg_pop,
            filename = "gbr_ppp_2020UKnatgrid100m.asc",
            datatype = "FLT4S",
            overwrite = TRUE)  
#crude plot of values if needed
#plot(reg_pop)

#resample up to 1km
reg_pop1km=aggregate(reg_pop,fact=c(10,10),fun=sum,na.rm=TRUE)
NAvalue(reg_pop1km) = -9999
writeRaster(x = reg_pop1km,
            filename = "gbr_ppp_2020UKnatgrid1km.tif",
            datatype = "FLT4S",
            options = c("COMPRESS=DEFLATE"),
            overwrite = TRUE)
writeRaster(x = reg_pop1km,
            filename = "gbr_ppp_2020UKnatgrid1km.asc",
            datatype = "FLT4S",
            overwrite = TRUE,
            prj=TRUE)
#and to 10km
reg_pop10km=aggregate(reg_pop1km,fact=c(10,10),fun=sum,na.rm=TRUE)
NAvalue(reg_pop10km) = -9999
writeRaster(x = reg_pop10km,
            filename = "gbr_ppp_2020UKnatgrid10km.tif",
            datatype = "FLT4S",
            options = c("COMPRESS=DEFLATE"),
            overwrite = TRUE,
            prj=TRUE)
writeRaster(x = reg_pop10km,
            filename = "gbr_ppp_2020UKnatgrid10km.asc",
            datatype = "FLT4S",
            overwrite = TRUE,
            prj=TRUE)         
         
         
         